import mindspore
from mindspore import nn, context
from mindspore.dataset import vision, transforms
from mindspore.dataset import MnistDataset
import os
import random
from mindspore.common.api import jit
from mindspore.common.parameter import Parameter
import numpy as np
from msprobe.mindspore import PrecisionDebugger
import sys

file_path = os.path.abspath(__file__)
src_directory = os.path.dirname(file_path)
msprobe_ms_grad_probe_dir = os.path.dirname(src_directory)

print(sys.argv)
level = int(sys.argv[-1])

json_path = os.path.join(msprobe_ms_grad_probe_dir, "json", f"L{level}_config.json")
debugger = PrecisionDebugger(json_path)

np.random.seed(42)

class Network(nn.Cell):
    def __init__(self):
        super().__init__()
        self.flatten = nn.Flatten()
        self.dense = nn.Dense(28*28, 10)
        self.dense_relu_sequential = nn.SequentialCell(
            nn.Dense(28*28, 512),
            nn.ReLU(),
            nn.Dense(512, 512),
            nn.ReLU(),
            nn.Dense(512, 10)
        )

    def construct(self, x):
        # x.shape (bs, 1, 28, 28)
        x = self.flatten(x)
        logits = self.dense_relu_sequential(x)
        return logits

model = Network()
fix_input = mindspore.Tensor(np.random.rand(4, 1, 28, 28), dtype=mindspore.float32)
fix_label = mindspore.Tensor([1, 3, 5, 7], dtype=mindspore.int32)


learning_rate = 1e-2
step = 10

loss_fn = nn.CrossEntropyLoss()
optimizer = nn.SGD(model.trainable_params(), learning_rate=learning_rate)
debugger.monitor(optimizer)


# Define forward function
def forward_fn(data, label):
    logits = model(data)
    loss = loss_fn(logits, label)
    return loss, logits

# Get gradient function
grad_fn = mindspore.value_and_grad(forward_fn, None, optimizer.parameters, has_aux=True)

def train_step(data, label):
    (loss, _), grads = grad_fn(data, label)
    is_overflow = False
    optimizer(grads)
    return loss, is_overflow


def train_loop(model):
    size = step
    model.set_train()
    for i in range(step):
        data = fix_input
        label = fix_label
        loss, is_overflow = train_step(data, label)

        loss = loss.asnumpy()
        print(f"loss: {loss:>7f}  [{i:>3d}/{size:>3d}] overflow: {is_overflow}")

train_loop(model)

print("Done!")

