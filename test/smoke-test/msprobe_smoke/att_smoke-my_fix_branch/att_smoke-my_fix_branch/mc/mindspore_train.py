from msprobe.mindspore import PrecisionDebugger
import platform
import mindspore as ms
import mindspore.ops as ops
from mindspore import nn
import mindspore.ops.functional as F
import numpy as np
import mindspore.dataset as ds
from mindspore.train import LossMonitor
from mindspore import Tensor


ms.set_context(mode=0, device_target='Ascend', jit_level="O2")
debugger = PrecisionDebugger(config_path="./config_tensor.json")
debugger.start()
class Net(ms.nn.Cell):
    def __init__(self):
        super().__init__()
        self.a = 1
        self.bias1 = ms.Parameter(ms.Tensor(np.array([-1], np.float32)))
        self.bias2 = ms.Parameter(ms.Tensor(np.array([-1], np.float32)))
        self.bias3 = ms.Parameter(ms.Tensor(np.array([-1], np.float32)))

    def construct(self, x):
        x -= self.bias1
        return x

class Loss(ms.nn.Cell):
    def __init__(self):
        super().__init__()

    def construct(self, x, y):
        return x - y

def generator():
    for _ in range(5):
        yield (3*np.ones([1]).astype(np.float32), 3*np.ones([1]).astype(np.int32))


if __name__ == '__main__':
    net = Net()
    optimizer = nn.Momentum(net.trainable_params(), 1, 0.9)
    loss = Loss()
    data = ds.GeneratorDataset(generator, ["data", "label"])
    model = ms.Model(net, loss, optimizer)
    lm = LossMonitor()
    model.train(1, data, callbacks=lm, dataset_sink_mode=True, sink_size=1)


