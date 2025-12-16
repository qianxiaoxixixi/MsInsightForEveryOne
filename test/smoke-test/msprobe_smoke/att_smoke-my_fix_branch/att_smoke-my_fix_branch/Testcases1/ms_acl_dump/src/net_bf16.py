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

x1 = Tensor([1, 2, 3], dtype=ms.float16)
x2 = Tensor([4, 5, 6], dtype=ms.float16)

net = Net()

# grad_fn = value_and_grad(net, grad_position=0)
# output, inputs_grad = grad_fn(x1, x2)
output = net(x1, x2)
print('TRAIN END')
