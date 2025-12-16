import os
import random
import sys

config_path = sys.argv[1]
device_id = int(sys.argv[2])

from msprobe.mindspore import PrecisionDebugger

import mindspore as ms
from mindspore import nn, ops, mint
from mindspore.dataset import vision, transforms
from mindspore.dataset import MnistDataset
from mindspore.train import Model, LossMonitor, Callback

os.environ["PYTHONHASHSEED"] = "1"
os.environ["HCCL_DETERMINISTIC"] = "true"
ms.set_seed(1)
random.seed(1)
ms.set_context(mode=ms.GRAPH_MODE, device_target="Ascend", device_id=device_id, deterministic="ON", jit_config={"jit_level": "O2"})

debugger = PrecisionDebugger(config_path=config_path)
debugger.start()


class StopStepMonitor(Callback):
    def __init__(self, stop_step):
        super(StopStepMonitor, self).__init__()
        self.stop_step = stop_step

    def on_train_step_end(self, run_context):
        cb_params = run_context.original_args()
        step_num = cb_params.cur_step_num    # 获取step值

        if step_num >= self.stop_step:
            run_context.request_stop()


def datapipe(path, batch_size):
    image_transforms = [
        vision.Rescale(1.0 / 255.0, 0),
        vision.Normalize(mean=(0.1307,), std=(0.3081,)),
        vision.HWC2CHW()
    ]
    label_transform = transforms.TypeCast(ms.int32)

    dataset = MnistDataset(path)
    dataset = dataset.map(image_transforms, 'image')
    dataset = dataset.map(label_transform, 'label')
    dataset = dataset.batch(batch_size)
    return dataset


class Network(nn.Cell):
    def __init__(self):
        super().__init__()
        self.net = Net()
        self.dense1 = nn.Dense(512, 512)
        self.dense2 = nn.Dense(512, 10)

    def construct(self, x):
        x = self.net(x)
        x = self.dense1(x)
        x = ops.relu(x)
        x = x.add(1.0)
        x = mint.sub(x, 1.0)
        logits = self.dense2(x)
        return logits


class Net(nn.Cell):
    def __init__(self):
        super().__init__()
        self.flatten = ops.Flatten()
        self.dense = nn.Dense(28*28, 512)
        self.relu = ops.ReLU()

    def construct(self, x):
        x = self.flatten(x)
        x = self.dense(x)
        x = self.relu(x)
        return x


test_dataset = datapipe('/data00/Dataset/MNIST_Data/test', 64*8)

model = Network()

loss_fn = nn.CrossEntropyLoss()
optimizer = nn.SGD(model.trainable_params(), 1e-2)
trainer = Model(model, loss_fn=loss_fn, optimizer=optimizer, metrics={'accuracy'})

loss_monitor = LossMonitor(1)
stop_step_monitor = StopStepMonitor(2)
trainer.train(1, test_dataset, callbacks=[loss_monitor, stop_step_monitor])
print("TRAIN END")

if "PYTHONHASHSEED" in os.environ:
    del os.environ["PYTHONHASHSEED"]

if "HCCL_DETERMINISTIC" in os.environ:
    del os.environ["HCCL_DETERMINISTIC"]
