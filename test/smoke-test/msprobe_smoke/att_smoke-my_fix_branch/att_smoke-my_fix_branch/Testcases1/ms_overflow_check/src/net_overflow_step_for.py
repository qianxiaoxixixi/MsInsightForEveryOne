import os
import random

import mindspore as ms
from mindspore import nn, ops, mint, Tensor
from mindspore.dataset import vision, transforms
from mindspore.dataset import MnistDataset
from mindspore.train import Model
import numpy as np

from msprobe.mindspore import PrecisionDebugger
cur_dir = os.path.dirname(os.path.realpath(__file__))
config_path = os.path.join(cur_dir, "..", "config", "config.json")
debugger = PrecisionDebugger(config_path=config_path)

ms.set_context(mode=ms.PYNATIVE_MODE, device_target="Ascend", device_id=5)

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
        tensor1 = Tensor(-3.4e+38, dtype=ms.float32)
        y1 = tensor1 * 2.0
        y2 = ops.relu(y1)
        y3 = mint.sqrt(tensor1)

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
model.set_train(True)

loss_fn = nn.CrossEntropyLoss()
optimizer = nn.SGD(model.trainable_params(), 1e-2)
trainer = Model(model, loss_fn=loss_fn, optimizer=optimizer, metrics={'accuracy'})


def forward_fn(inputs, targets):
    logits = model(inputs)
    loss = loss_fn(logits, targets)
    return loss

grad_fn = ms.value_and_grad(forward_fn, None, optimizer.parameters)


def train_step(inputs, targets):
    debugger.start()
    loss, grads = grad_fn(inputs, targets)
    optimizer(grads)
    debugger.stop()
    debugger.step()
    return loss


def train_epoch(data_loader, epoch):
    losses = []
    model.set_train(True)

    for i, (images, labels) in enumerate(data_loader):
        loss = train_step(images, labels)
        print('Epoch: [%3d/%3d], Steps: [%3d], Train Loss: [%5.3f]' %
                (epoch + 1, num_epochs, i + 1, loss))
        losses.append(loss)

    return sum(losses) / len(losses)

num_epochs = 1
data_loader = test_dataset.create_tuple_iterator(num_epochs=num_epochs)

for epoch in range(num_epochs):
    epoch_average_loss = train_epoch(data_loader, epoch)
    print("Epoch: [%3d/%3d], Average Train Loss: [%5.3f]" % (
        epoch+1, num_epochs, epoch_average_loss
    ))

print("TRAIN END")
