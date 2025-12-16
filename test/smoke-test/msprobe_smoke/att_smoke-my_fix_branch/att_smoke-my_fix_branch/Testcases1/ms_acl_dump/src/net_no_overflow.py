import sys

import mindspore as ms
from mindspore import ops, nn, Tensor, value_and_grad
from msprobe.mindspore import PrecisionDebugger

config_path = sys.argv[1]
device_id = int(sys.argv[2])

ms.set_context(mode=ms.GRAPH_MODE, device_target="Ascend", device_id=device_id, deterministic="ON", jit_config={"jit_level": "O2"})
print(ms.get_context('jit_config'))

debugger = PrecisionDebugger(config_path=config_path)
debugger.start()


class Net(nn.Cell):
    def construct(self, x1, x2):
        logits = ops.mul(x1, x2)
        return logits

x1 = Tensor([-1.7e+38], dtype=ms.float32)
x2 = Tensor([2], dtype=ms.float32)

net = Net()

output = net(x1, x2)
print('TRAIN END')
