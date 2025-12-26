import os
import random

import mindspore as ms
from mindspore import nn, ops, mint
from mindspore.dataset import vision, transforms
from mindspore.dataset import MnistDataset
from mindspore.train import Model
from mindspore.train import LossMonitor
import numpy as np

ms.set_context(mode=ms.GRAPH_MODE, device_target="Ascend", device_id=5, jit_config={"jit_level": "O0"})

ms.set_seed(1)
np.random.seed(1)
random.seed(1)


def datapipe(path, batch_size):
    image_transforms = [
        vision.Rescale(1.0 / 255.0, 0),
        vision.Normalize(mean=(0.1307,), std=(0.3081,)),
        vision.HWC2CHW()
    ]
    label_transform = transforms.TypeCast(ms.int32)

    dataset = MnistDataset(path)
    dataset = dataset.map(image_transforms, 'image')
    dataset = dataset.map(label_transform, 'label')
    dataset = dataset.batch(batch_size)
    return dataset


class Network(nn.Cell):
    def __init__(self):
        super().__init__()
        self.net = Net()
        self.dense1 = nn.Dense(512, 512)
        self.dense2 = nn.Dense(512, 10)

    def construct(self, x):
        x = self.net(x)
        x = self.dense1(x)
        x = ops.relu(x)
        x = x.add(1.0)
        x = mint.sub(x, 1.0)
        logits = self.dense2(x)
        return logits


class Net(nn.Cell):
    def __init__(self):
        super().__init__()
        self.flatten = ops.Flatten()
        self.dense = nn.Dense(28*28, 512)
        self.relu = ops.ReLU()

    def construct(self, x):
        x = self.flatten(x)
        x = self.dense(x)
        x = self.relu(x)
        return x


test_dataset = datapipe('/data00/Dataset/MNIST_Data/test', 64*8)

model = Network()
ms.set_dump(model.net.relu)

loss_fn = nn.CrossEntropyLoss()
optimizer = nn.SGD(model.trainable_params(), 1e-2)
trainer = Model(model, loss_fn=loss_fn, optimizer=optimizer, metrics={'accuracy'})

loss_monitor = LossMonitor(10)
trainer.train(1, test_dataset, callbacks=[loss_monitor])
print("TRAIN END")
