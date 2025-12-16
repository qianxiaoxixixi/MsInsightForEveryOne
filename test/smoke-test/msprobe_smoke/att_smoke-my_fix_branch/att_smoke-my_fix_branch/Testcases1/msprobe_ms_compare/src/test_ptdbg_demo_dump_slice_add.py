import sys
import torch
import torch.nn as nn
import torch.nn.functional as F
# from msprobe.pytorch import *

from msprobe.pytorch.common import seed_all
from msprobe.pytorch import PrecisionDebugger

torch.npu.set_device("npu:7")

class Net(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc = nn.Linear(3, 3)

    def forward(self, input):
        output = self.fc(input)
        output += input
        output = output[1, :]
        return output


seed_all(mode=True)
net = Net()
input = torch.randn(2, 3)
# output_path = sys.argv[1]
# set_dump_path(output_path)
# register_hook(net, acc_cmp_dump)
# set_dump_switch("ON")
config_json = sys.argv[1]
debugger = PrecisionDebugger(config_json, model=net)
debugger.start()
output = net(input)

debugger.stop()
# set_dump_switch("OFF")
