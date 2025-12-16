import time

start_time = time.time()

import os
import random
import sys

import mindspore as ms
from mindspore import nn, ops, mint
from mindspore.dataset import vision, transforms
from mindspore.dataset import MnistDataset

from msprobe.mindspore import PrecisionDebugger

config_path = sys.argv[1]
device_id = int(sys.argv[2])

os.environ["PYTHONHASHSEED"] = "1"
os.environ["HCCL_DETERMINISTIC"] = "true"
ms.set_seed(1)
random.seed(1)
ms.set_context(mode=ms.GRAPH_MODE, device_target="Ascend", device_id=device_id, deterministic="ON", jit_config={"jit_level": "O2"})


debugger = PrecisionDebugger(config_path=config_path)


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
model.set_train(True)

loss_fn = nn.CrossEntropyLoss()
optimizer = nn.SGD(model.trainable_params(), 1e-2)


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

end_time = time.time()

print(f"TRAIN END, spend: {end_time - start_time}", flush=True)

if "PYTHONHASHSEED" in os.environ:
    del os.environ["PYTHONHASHSEED"]

if "HCCL_DETERMINISTIC" in os.environ:
    del os.environ["HCCL_DETERMINISTIC"]

if "DEVICE_ID" in os.environ:
    del os.environ["DEVICE_ID"]
