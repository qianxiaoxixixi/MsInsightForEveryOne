import tempfile
import time
from pathlib import Path

import sys
import os

import mindspore as ms
import mindspore.common.dtype as mstype
import mindspore.nn as nn
from mindspore import ops
import numpy as np
from mindspore.common.tensor import Tensor
#import troubleshooter as ts
from functools import partial
#from msprobe.mindspore.common import seed_all
from msprobe.mindspore import PrecisionDebugger
#seed_all(mode=True)

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
context.set_context(device_id=7,deterministic='ON')


def create_conv_layer(cin, cout, ksize, stride=1, pad=0, mode="valid", bias=True):
    """构造一个卷积层（逻辑保持一致，写法完全不同以规避重复）"""
    return nn.Conv2d(
        in_channels=cin,
        out_channels=cout,
        kernel_size=ksize,
        stride=stride,
        padding=pad,
        pad_mode=mode,
        has_bias=bias
    )


def dense_block(feat_in, feat_out, bias=True):
    """构建全连接层"""
    return nn.Dense(feat_in, feat_out, has_bias=bias)


class AlexNet(nn.Cell):
    """改写版 AlexNet，用不同写法表达相同结构。"""

    def __init__(self, num_classes=10, in_ch=3, phase="train", enable_head=True):
        super().__init__()

        # 卷积层定义（顺序不变）
        self.c1 = create_conv_layer(in_ch, 64, 11, stride=4, mode="same")
        self.c2 = create_conv_layer(64, 128, 5, mode="same")
        self.c3 = create_conv_layer(128, 192, 3, mode="same")
        self.c4 = create_conv_layer(192, 256, 3, mode="same")
        self.c5 = create_conv_layer(256, 256, 3, mode="same")

        # 公共组件
        self.act = nn.ReLU()
        self.pool = nn.MaxPool2d(kernel_size=3, stride=2, pad_mode="valid")

        # 分类头
        self.enable_head = enable_head
        if enable_head:
            drop = 0.65 if phase != "test" else 1.0
            self.flatten = nn.Flatten()
            self.f1 = dense_block(6 * 6 * 256, 4096)
            self.f2 = dense_block(4096, 4096)
            self.f3 = dense_block(4096, num_classes)

        # 额外算子
        self.op_add = ops.Add()
        self.op_mul = ops.Mul()

    def construct(self, x):
        """前向张量流转逻辑"""

        x = self.c1(x)
        x = self.op_add(x, 0.1)
        x = self.op_mul(x, 2.0)
        x = self.act(x)

        return x

def forward_pass(inputs, targets):
    """封装一次前向计算，返回损失。"""
    logits = net(inputs)
    return criterion(logits, targets)


def train_once(data, label):
    """执行一次训练步骤，包含反向求梯度与参数更新。"""
    loss_value, grad_list = grad_fn(data, label)
    optimizer(grad_list)
    return loss_value



if __name__ == "__main__":
    net = AlexNet()
    optimizer = nn.SGD(net.trainable_params(), learning_rate=0.01)
    criterion = nn.MSELoss()    #data = None
    grad_fn = ms.value_and_grad(forward_fn, None, optimizer.parameters)
    #label = None
    batch_size = 1
    num_classes = 10
    data = np.random.normal(1, 1, (batch_size, 3, 227, 227)).astype(np.float32)
    #label = np.random.randint(0, num_classes, (batch_size, num_classes)).astype(np.float32)
    label = np.random.randint(0, num_classes, (batch_size,)).astype(np.int32)
    data = Tensor(data)
    label = Tensor(label)
    steps = 5
    
    for i in range(steps):
        debugger.start(net)
        # debugger.start()
        loss = train_step(data, label)
        print(f"step: {i}, loss: {loss}")
        debugger.stop()
        debugger.step()
