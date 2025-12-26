# 根据需要import包
import os
import sys
import torch
import torch.nn as nn
import torch_npu

torch.npu.set_device("npu:7")

from msprobe.pytorch.common import seed_all
from msprobe.pytorch import PrecisionDebugger


# 定义一个简单的网络
class ModuleOP(nn.Module):

    def __init__(self) -> None:
        super().__init__()
        self.linear_1 = nn.Linear(in_features=2, out_features=2)
        self.linear_2 = nn.Linear(in_features=2, out_features=1)
        self.relu = nn.ReLU()

    def forward(self, x):
        x1 = self.linear_1(x)
        x2 = self.linear_2(x1)
        r1 = self.relu(x2)
        return r1


if __name__ == "__main__":
    seed_all(mode=True)
    config_json = sys.argv[1]

    module = ModuleOP()
    debugger = PrecisionDebugger(config_json, model=module)
    debugger.start()

    # dump_mode = ['backward', 'output']

    x = torch.randn(2, 2)

    module.npu()
    x = x.npu()
    out = module(x)
    loss = out.sum()
    loss.backward()

    debugger.stop()
