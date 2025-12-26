# Copyright 2024 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================
""" pp train """
import os
import time
import random
import argparse
import numpy as np
import mindspore as ms
from mindspore.ops import operations as P
from mindspore.communication.management import init
from mindformers.wrapper import MFTrainOneStepCell
from mindformers.core.loss import CrossEntropyLoss
from mindformers.experimental.distri_cores.config import init_arguments
from mindformers.experimental.distri_cores.pipeline_parallel import pipelining_1F1B_without_interleaved, \
                                                                    PipelineCell
from training_overflow import PipelineTrainOneStepCell
from mindformers.experimental.distri_cores.create_comm import initialize_model_parallel, get_pp_world_size, \
                                                              get_dp_world_size, get_dp_group, get_dp_rank, \
                                                              get_tp_rank, get_tp_world_size

from mindformers.experimental.pangu_demo.pp_model import PanGUAlphaWithPipeline
from mindformers.experimental.pangu_demo.get_optimizer import get_optimizer
from mindformers.experimental.pangu_demo.dataset_new import get_dataset
from mindformers.experimental.pangu_demo.utils import set_seed

# ms.set_context(pynative_synchronize=True)
np.random.seed(1)
random.seed(1)

try:
    import torch
except:
    pass

from msprobe.mindspore import PrecisionDebugger
cur_dir = os.path.dirname(os.path.realpath(__file__))
config_path = os.path.join(cur_dir, "..", "..", "config", "config.json")
debugger = PrecisionDebugger(config_path=config_path)


def transform_tp_params(params, dtype=np.float32):
    """ transform standalone ckpt to tp ckpt"""
    tp_rank = get_tp_rank()
    tp_world_size = get_tp_world_size()
    new_params = {}
    for name, param in params.items():
        param = param.asnumpy()
        if "embedding" in name:
            start = tp_rank * (param.shape[0] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[0] // tp_world_size)
            new_param = param[start: end, :]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "qkv_proj.weight" in name:
            q = param[:, :param.shape[1] // 3]
            k = param[:, param.shape[1] // 3: 2 * param.shape[1] // 3]
            v = param[:, 2 * param.shape[1] // 3:]
            start = tp_rank * (q.shape[1] // tp_world_size)
            end = (tp_rank + 1) * (q.shape[1] // tp_world_size)
            new_param = np.concatenate([q[:, start: end], k[:, start: end], v[:, start: end]], axis=-1)
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "qkv_proj.bias" in name:
            q = param[:param.shape[0] // 3]
            k = param[param.shape[0] // 3: 2 * param.shape[1] // 3]
            v = param[2 * param.shape[0] // 3:]
            start = tp_rank * (q.shape[0] // tp_world_size)
            end = (tp_rank + 1) * (q.shape[0] // tp_world_size)
            new_param = np.concatenate([q[start: end], k[start: end], v[start: end]], axis=-1)
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "q_proj.weight" in name:
            start = tp_rank * (param.shape[1] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[1] // tp_world_size)
            new_param = param[:, start: end]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "kv_proj.weight" in name:
            k = param[:, :param.shape[1] // 2]
            v = param[:, param.shape[1] // 2: 2 * param.shape[1] // 2]
            start = tp_rank * (k.shape[1] // tp_world_size)
            end = (tp_rank + 1) * (k.shape[1] // tp_world_size)
            new_param = np.concatenate([q[:, start: end], k[start: end], v[start: end]], axis=-1)
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "matmul.weight" in name:
            start = tp_rank * (param.shape[0] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[0] // tp_world_size)
            new_param = param[start: end, :]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "out_proj.weight" in name:
            start = tp_rank * (param.shape[0] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[0] // tp_world_size)
            new_param = param[start: end, :]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "mlp.mapping.weight" in name:
            start = tp_rank * (param.shape[1] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[1] // tp_world_size)
            new_param = param[:, start: end]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "mlp.mapping.bias" in name:
            start = tp_rank * (param.shape[0] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[0] // tp_world_size)
            new_param = param[start: end]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        elif "projection.weight" in name:
            start = tp_rank * (param.shape[0] // tp_world_size)
            end = (tp_rank + 1) * (param.shape[0] // tp_world_size)
            new_param = param[start: end]
            new_params[name] = ms.Parameter(new_param.astype(dtype))
        else:
            new_param = param
            new_params[name] = ms.Parameter(new_param.astype(dtype))
    return new_params


def train(
        epochs,
        dataset,
        network,
        optimizer,
        config,
        max_steps=None
):
    """
    Trains a neural network model using the given dataset and optimizer.
    """
    micro = config.parallel_config.micro_batch_num
    if get_pp_world_size() > 1:
        train_one_step_cell = PipelineTrainOneStepCell(network,
                                                       optimizer,
                                                       config,
                                                       use_clip_grad=False,
                                                       max_grad_norm=1.0,
                                                       scale_sense=1.0,
                                                       micro_batch_num=micro)
    else:
        train_one_step_cell = MFTrainOneStepCell(network,
                                                 optimizer,
                                                 use_clip_grad=False,
                                                 max_grad_norm=1.0,
                                                 scale_sense=1.0)
    network.set_train()
    network.set_inputs()

    total_step = 1
    for epoch in range(epochs):
        for step, (input_ids, input_position, attention_mask) in enumerate(
                dataset.create_tuple_iterator()
        ):
            debugger.start()
            start_time = time.time()
            if get_pp_world_size() > 1:
                if get_dp_world_size() > 1:
                    dp_rank = get_dp_rank()
                    batch_size = config.training.batch_size
                    input_ids = input_ids[dp_rank * batch_size * micro: (dp_rank + 1) * batch_size * micro]
                    input_position = input_position[dp_rank * batch_size * micro: (dp_rank + 1) * batch_size * micro]
                    attention_mask = attention_mask[dp_rank * batch_size * micro: (dp_rank + 1) * batch_size * micro]
                loss, overflow, loss_scale, learning_rate = train_one_step_cell(pipelining_1F1B_without_interleaved,
                                                                                input_ids,
                                                                                input_position,
                                                                                attention_mask)
                if config.parallel_config.reduction == "mean":
                    loss /= micro
            else:
                if get_dp_world_size() > 1:
                    dp_rank = get_dp_rank()
                    batch_size = config.training.batch_size
                    input_ids = input_ids[dp_rank * batch_size: (dp_rank + 1) * batch_size]
                    input_position = input_position[dp_rank * batch_size: (dp_rank + 1) * batch_size]
                    attention_mask = attention_mask[dp_rank * batch_size: (dp_rank + 1) * batch_size]
                loss, overflow, loss_scale, learning_rate = train_one_step_cell(input_ids,
                                                                                input_position,
                                                                                attention_mask)
                if get_dp_world_size() > 1:
                    loss = P.AllReduce(group=get_dp_group())(loss)
                    if config.parallel_config.reduction == "mean":
                        loss /= get_dp_world_size()
            if loss.shape == (1,):
                loss = loss[0]
            end_time = time.time()
            print(f"显存峰值{round(ms.hal.max_memory_allocated(device_target='Ascend') / 1024 / 1024 / 1024, 2)}GB", flush=True)
            print("Epoch: {} | Step: {} | Loss: {} | Overflow cond: {} | lr: {} | loss scale: {} | cost time: {}" \
                  .format(epoch, step + 1, loss, overflow,
                          learning_rate, loss_scale, end_time - start_time),
                  flush=True)

            if max_steps is not None:
                if total_step == max_steps:
                    exit()
            total_step += 1
            debugger.stop()
            debugger.step()


def main(config_path):
    """main process"""
    # init context
    ms.set_context(device_target="Ascend", mode=ms.PYNATIVE_MODE, deterministic="ON")
    # init parallel context
    config = init_arguments(config_path)

    init()
    initialize_model_parallel(
        tp_size=config.parallel_config.tensor_parallel,
        pp_size=config.parallel_config.pipeline_stage,
        ep_size=config.parallel_config.expert_parallel,
        cp_size=config.parallel_config.context_parallel,
    )

    # set up seed
    set_seed(config.seed)

    # define network, loss_fn, lr_scheduler, optimizer, dataset
    loss_fn = CrossEntropyLoss()
    network = PanGUAlphaWithPipeline(
        pad_token=config.train_dataset.eod_id,
        loss_fn=loss_fn,
        config=config.model_config,
    )

    ## load ckpt
    # ckpt = ms.load_checkpoint('./ckpt/standalone_weight.ckpt')
    # if get_tp_world_size() > 1:
    #     ckpt = transform_tp_params(ckpt)
    # not_loaded_params = ms.load_param_into_net(network, ckpt)
    # print(f"not loaded params: {not_loaded_params}", flush=True)

    pangu_map = {
        'preprocess': network.preprocess,
        'embedding': network.model.backbone.embedding,
        'transformer_layers': network.model.backbone.transformer_layers.layers,
        'final_norm': network.model.backbone.transformer_layers.final_norm,
        'head': network.model.head,
        'loss': network.loss,
    }
    if get_pp_world_size() > 1:
        network = PipelineCell(network, pangu_map)
    optimizer = get_optimizer(config.optimizer, config.lr_scheduler, network.trainable_params())
    dataset = get_dataset(config)

    # train
    train(
        epochs=config.training.epochs,
        dataset=dataset,
        network=network,
        optimizer=optimizer,
        config=config,
        max_steps=config.training.max_steps
    )


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--run_mode', type=str, default='pp',
        help="pp: run pp process \
              other: run other process."
    )
    args = parser.parse_args()
    if args.run_mode == "pp":
        config_file = "pangu_alpha.yaml"
    else:
        config_file = "pangu_alpha_others.yaml"
    main(config_file)

