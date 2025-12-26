import sys
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch_npu

from msprobe.pytorch.common import seed_all
from msprobe.pytorch import PrecisionDebugger

torch.npu.set_device("npu:7")


class Net(nn.Module):
    def __init__(self):
        super().__init__()
        self.fc = nn.Linear(3, 3)

    def forward(self, input):
        print(input)
        output = F.dropout(input, p=0.8)
        print(output)
        return output


if __name__ == '__main__':
    seed_all(mode=True)
    config_json = sys.argv[1]

    net = Net()

    debugger = PrecisionDebugger(config_json, model=net)
    debugger.start()

    input = torch.randn(2, 3)
    output = net(input)

    if torch.equal(input, output):
        print("dropout is 0")
    else:
        print("[ERROR]dropout is not 0")
    debugger.stop()
