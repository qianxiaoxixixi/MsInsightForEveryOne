# 根据需要import包
import os
import sys
import torch
import torch.nn as nn
import torch_npu

from msprobe.pytorch.common import seed_all
from msprobe.pytorch import PrecisionDebugger

torch.npu.set_device("npu:7")


# 定义一个简单的网络
class ModuleOP(nn.Module):

    def __init__(self):
        super(ModuleOP, self).__init__()
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=16, kernel_size=5, stride=1, padding=2)
        self.relu1 = nn.ReLU()
        self.bn1 = nn.BatchNorm2d(16)
        self.pool1 = nn.MaxPool2d(kernel_size=2)
        self.conv2 = nn.Conv2d(in_channels=16, out_channels=32, kernel_size=5, stride=1, padding=2)
        self.relu2 = nn.ReLU()
        self.pool2 = nn.MaxPool2d(2)
        self.fc = nn.Linear(32 * 7 * 7, 2)

    def forward(self, x):
        x = self.conv1(x)

        x = self.bn1(x)
        x = self.relu1(x)
        x = self.pool1(x)
        x = self.conv2(x)

        x = self.relu2(x)
        x = self.pool2(x)

        x = x.view(x.size(0), -1)
        output = self.fc(x)
        return output


if __name__ == "__main__":
    print('democnn')
    seed_all(mode=True)
    module = ModuleOP()
    # 对模型注入forwar和backward的hooks，采样功能默认开启，如需关闭，请使用register_hook(model_cpu, wrap_acc_cmp_hook, sample=False)
    debugger = PrecisionDebugger(sys.argv[1], model=module)
    debugger.start()
    x = torch.randn(1, 1, 28, 28)

    # npu上计算，dump数据
    module.npu()
    x = x.npu()
    out = module(x)
    loss = out.sum()
    loss.backward()

    debugger.stop()
