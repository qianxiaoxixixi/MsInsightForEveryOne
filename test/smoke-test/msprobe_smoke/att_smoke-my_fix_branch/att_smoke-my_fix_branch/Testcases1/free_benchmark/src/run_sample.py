import torch.nn as nn
from torchvision.models import resnet18
import torch
import sys
import os
from msprobe.pytorch import PrecisionDebugger
from msprobe.pytorch.common import seed_all

import torch.nn.functional as F
import torch_npu

torch.npu.set_device("npu:1")

seed_all(mode=True)

config_json = sys.argv[1]


class ComplexModel(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(ComplexModel, self).__init__()
        self.linear_real = nn.Linear(input_dim, output_dim)
        self.linear_imag = nn.Linear(input_dim, output_dim)

    def forward(self, x_real, x_imag):
        x_real = self.linear_real(x_real)
        x_imag = self.linear_real(x_imag)
        x_real_activated = F.relu(x_real)
        x = x_real_activated + x_imag * 1j
        return x


with torch.autograd.set_detect_anomaly(True):
    model = resnet18().to(dtype=torch.bfloat16)  # .npu()
    x = torch.randn(1, 3, 128, 128).to(dtype=torch.bfloat16)  # .npu()

    debugger = PrecisionDebugger(config_json)
    debugger.start()

    y = model(x)
    target = torch.tensor([100], dtype=torch.long)
    loss = torch.nn.CrossEntropyLoss()(y, target)
    loss.backward()

    # 复数模型
    x_real = torch.randn(10, 256, requires_grad=True).to(
        dtype=torch.bfloat16)  # 输入复数张量
    x_imag = torch.randn(10, 256, requires_grad=True).to(
        dtype=torch.bfloat16)  # 输入复数张量
    model = ComplexModel(256, 20).to(dtype=torch.bfloat16)
    output = model(x_real, x_imag)  # 正确处理复数输入
    target_real = torch.randn(10, 20)
    target_imag = torch.randn(10, 20)
    target = target_real + target_imag*1j
    loss = nn.MSELoss()(output.real, target.real) + \
        nn.MSELoss()(output.imag, target.imag)
    loss.backward()

    debugger.stop()

