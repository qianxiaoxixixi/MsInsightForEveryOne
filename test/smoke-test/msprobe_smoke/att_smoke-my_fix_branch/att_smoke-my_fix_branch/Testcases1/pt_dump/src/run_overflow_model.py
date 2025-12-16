import sys

import torch
import torch.nn as nn
from msprobe.pytorch import PrecisionDebugger
from msprobe.pytorch.common.utils import seed_all

import torch_npu

CALCULATE_DEVICE = "npu:6"
device = torch.device(CALCULATE_DEVICE)

seed_all(mode=True)

config_json = sys.argv[1]


class SubChildModule(nn.Module):  
    def __init__(self):  
        super(SubChildModule, self).__init__()  
        self.tensor = torch.tensor([1e+38], dtype=torch.float32).to(device)

    def forward(self, value):  
        result = torch.add(self.tensor, value)
        return result


class ChildModule(nn.Module):  
    def __init__(self):  
        super(ChildModule, self).__init__()  
        self.tensor = torch.tensor([1e+38], dtype=torch.float32).to(device)
        self.sub_child = SubChildModule()

    def forward(self, value):  
        result = torch.add(self.tensor, value)
        return self.sub_child(result)


class ParentModule(nn.Module):  
    def __init__(self):  
        super(ParentModule, self).__init__()  
        self.child = ChildModule()

    def forward(self, value):
        return self.child(value) 


def ModelTrain():
    module = ParentModule().to(device)

    debugger = PrecisionDebugger(config_json, model=module)

    debugger.start()
    max_value = torch.finfo(torch.float32).max
    input_tensor = torch.tensor([max_value], dtype=torch.float32, requires_grad=True).to(device)
    output = module(input_tensor)
    loss = output.sum() 
    loss.backward()
    debugger.stop()


if __name__ == "__main__":
    ModelTrain()
