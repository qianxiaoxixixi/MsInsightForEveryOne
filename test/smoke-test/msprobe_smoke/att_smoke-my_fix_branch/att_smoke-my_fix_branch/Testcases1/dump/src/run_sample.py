import torch
import sys
import os
from msprobe.pytorch import PrecisionDebugger
from msprobe.pytorch.common import seed_all

import torch_npu

torch.npu.set_device("npu:7")

seed_all(mode=True)

config_json = sys.argv[1]


class MyModel(torch.nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = torch.nn.Linear(10, 10)
        self.relu = torch.nn.ReLU()

    def forward(self, x):
        breakpoint()
        y = self.linear(x)
        return self.relu(y)


class SampleModel(torch.nn.Module):
    def __init__(self):
        super().__init__()
        self.linear = torch.nn.Linear(10, 10)
        self.model = MyModel()

    def forward(self, x):
        return self.linear(self.model(x))


from torchvision.models import resnet18

with torch.autograd.set_detect_anomaly(True):
    model = resnet18()  # .npu()
    x = torch.randn(1, 3, 128, 128)  # .npu()

    debugger = PrecisionDebugger(config_json, model=model)
    debugger.start()

    y = model(x)
    y += 65536.
    target = torch.tensor([100], dtype=torch.long)
    loss = torch.nn.CrossEntropyLoss()(y, target)
    loss.backward()
    debugger.stop()
