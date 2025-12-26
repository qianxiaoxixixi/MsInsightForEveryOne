import os
import argparse
from enum import Enum
from mindspore import context
from mindspore import Tensor
import mindspore as ms
import numpy as np

import mindspore.dataset as ds
import mindspore.dataset.transforms.c_transforms as C
import mindspore.dataset.vision.c_transforms as CV
from mindspore.dataset.vision import Inter
from mindspore import dtype as mstype
from mindspore.train.callback import ModelCheckpoint, CheckpointConfig
import mindspore.nn as nn
from mindspore.common.initializer import Normal
from mindspore.train import Callback
from mindspore.train import LossMonitor
# 导入模型训练需要的库
from mindspore.nn import Accuracy
from mindspore.train.callback import LossMonitor
from mindspore import Model

from mindspore import JitConfig
jitconfig = JitConfig(jit_level="O1")
from msprobe.mindspore import PrecisionDebugger
import random
ms.set_seed(1)
np.random.seed(1)
random.seed(1)
# config_json = sys.argv[1]
#debugger = PrecisionDebugger(config_path = config_json)
import os
print(os.getcwd())
script_dir = os.path.dirname(os.path.abspath(__file__))
config_path = os.path.join(script_dir, 'config.json')

debugger = PrecisionDebugger(config_path=config_path)
from mindspore import context
context.set_context( device_id=5,deterministic='ON')

class TestCaseMode(Enum):
    ACL_DUMP = 0
    GRAD_MONITOR = 1


test_mode = None
if os.getenv("GRADDUMPPATH") is not None:
    from grad_tool.grad_monitor import GradientMonitor
    test_mode = TestCaseMode.GRAD_MONITOR
else:
    test_mode = TestCaseMode.ACL_DUMP


parser = argparse.ArgumentParser(description='MindSpore LeNet Example')
parser.add_argument('--device_target', type=str, default="Ascend", choices=['Ascend', 'GPU', 'CPU'])

args = parser.parse_known_args()[0]
context.set_context(mode=context.GRAPH_MODE, device_target=args.device_target, device_id=1)

def create_dataset(data_dir, batch_size=128, repeat_size=1, num_parallel_workers=4):
    """构造 MNIST 数据集，包含预处理和批处理流程。"""

    # 加载原始数据（取样数量 = batch_size * 3）
    dataset = ds.MnistDataset(data_dir, num_samples=batch_size * 3)

    # 图像缩放与归一化参数
    new_size = (32, 32)
    normalize_scale = 1.0 / 255.0
    norm_scale = 1 / 0.3081
    norm_shift = -0.1307 / 0.3081

    # 定义图像处理算子
    ops = [
        ("label", C.TypeCast(mstype.int32)),
        ("image", CV.Resize(new_size, interpolation=Inter.LINEAR)),
        ("image", CV.Rescale(normalize_scale, 0.0)),
        ("image", CV.Rescale(norm_scale, norm_shift)),
        ("image", CV.HWC2CHW())
    ]

    # 依次应用算子
    for col, op in ops:
        dataset = dataset.map(
            operations=op, input_columns=col, num_parallel_workers=num_parallel_workers
        )

    # 随机打乱与批处理
    dataset = dataset.shuffle(buffer_size=10000)
    dataset = dataset.batch(batch_size, drop_remainder=True)

    return dataset


class LeNet5(nn.Cell):
    """简化版 LeNet5 网络结构定义。"""

    def __init__(self, num_classes=10, in_channels=1):
        super().__init__()

        # 卷积层与池化层组合
        self.conv_block1 = nn.SequentialCell(
            nn.Conv2d(in_channels, 6, kernel_size=5, pad_mode='valid'),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2)
        )

        self.conv_block2 = nn.SequentialCell(
            nn.Conv2d(6, 16, kernel_size=5, pad_mode='valid'),
            nn.ReLU(),
            nn.MaxPool2d(kernel_size=2, stride=2)
        )

        # 全连接层
        self.flatten = nn.Flatten()
        self.fc_layers = nn.SequentialCell(
            nn.Dense(16 * 5 * 5, 120, weight_init=Normal(0.02)),
            nn.Dense(120, 84, weight_init=Normal(0.02)),
            nn.ReLU(),
            nn.Dense(84, num_classes, weight_init=Normal(0.02))
        )

    def construct(self, x):
        x = self.conv_block1(x)
        x = self.conv_block2(x)
        x = self.flatten(x)
        x = self.fc_layers(x)
        return x


# 实例化网络
net = LeNet5()

net.set_jit_config(jitconfig)
debugger.start()
# 定义损失函数
net_loss = nn.SoftmaxCrossEntropyWithLogits(sparse=True, reduction='mean')

# 定义优化器
net_opt = nn.Momentum(net.trainable_params(), learning_rate=0.1, momentum=0.9)

# 设置模型保存参数
config_ck = CheckpointConfig(save_checkpoint_steps=1875, keep_checkpoint_max=5)

def train_net(args, model, epoch_size, data_path, repeat_size, sink_mode):
    """定义训练的方法"""
    # 加载训练数据集
    ds_train = create_dataset(os.path.join(data_path, "train"), 32, repeat_size)
    loss_monitor = LossMonitor(1)
    model.train(epoch_size, ds_train, callbacks=[loss_monitor], dataset_sink_mode=sink_mode)


train_epoch = 2
mnist_path = "/home/maoyanlong/templenet/MNIST_Data"
dataset_size = 1
input_tensor = Tensor(np.ones([1, 1, 32, 32]).astype(np.float32))

# check testcase mode
if test_mode == TestCaseMode.GRAD_MONITOR:
    gm = GradientMonitor(os.getenv("GRADDUMPPATH"), framework="MindSpore")
    gm.monitor(net_opt)

model = Model(net, net_loss, net_opt, metrics={"Accuracy": Accuracy()})
# 训练模型
train_net(args, model, train_epoch, mnist_path, dataset_size, False)
if test_mode == TestCaseMode.GRAD_MONITOR:
    gm.stop()
print(".........Training Over!.............")
