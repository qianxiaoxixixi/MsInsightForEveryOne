import sys
from typing import Callable, Optional, Tuple

import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torchvision.transforms as transforms
from torch.utils.data import DataLoader, Dataset
from torchvision.datasets import FakeData

import torch_npu
from msprobe.pytorch import PrecisionDebugger
from msprobe.pytorch.common import seed_all

CALCULATE_DEVICE = "npu:1"
device = torch.device(CALCULATE_DEVICE)

seed_all(mode=True)

config_json = sys.argv[1]


class FakeDataSet(Dataset):
    def __init__(
            self,
            size: int = 40,
            image_size: Tuple[int, int, int] = (3, 244, 244),
            num_classes: int = 10,
            transform: Optional[Callable] = None
    ) -> None:
        self.fake_dataset = FakeData(size, image_size, num_classes)
        if transform is None:
            self.transform = transforms.ToTensor()
        else:
            self.transform = transform

        self.images = [self.transform(data[0]) for data in self.fake_dataset]
        self.labels = [data[1] for data in self.fake_dataset]

    def __len__(self):
        return len(self.images)

    def __getitem__(self, item):
        image = self.images[item]
        label = torch.from_numpy(np.array(self.labels[item], dtype=np.int64))
        return image, label


class LeNet(nn.Module):
    def __init__(self):
        super(LeNet, self).__init__()
        self.conv1 = nn.Conv2d(3, 16, 5)
        self.pool1 = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(16, 32, 5)
        self.pool2 = nn.MaxPool2d(2, 2)
        self.fc1 = nn.Linear(32 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = F.relu(self.conv1(x))
        x = self.pool1(x)
        x = F.relu(self.conv2(x))
        x = self.pool2(x)
        x = x.view(-1, 32 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


def train_one_epoch(data_loader, model, optimizer, criterion):
    model.train()

    if "test_pytorch_L0_statistics_none_precision_debugger" in config_json:
        debugger = PrecisionDebugger(config_path=config_json, task="statistics", level="L0", model=model)
    elif "test_pytorch_L1_tensor_rank_none_precision_debugger" in config_json:
        debugger = PrecisionDebugger(config_path=config_json, task="tensor", level="L1", step=[1])
    else: # test_pytorch_mix_statistics_none_precision_debugger
        debugger = PrecisionDebugger(config_path=config_json, task="statistics", level="mix", model=model)

    for image, label in data_loader:
        debugger.start()

        image, label = image.to(device), label.to(device)
        optimizer.zero_grad()
        output = model(image)
        loss = criterion(output, label)
        loss.backward()
        optimizer.step()

        debugger.stop()
        debugger.step()


def LeNetTrain():
    image_num = 40
    image_size = (3, 32, 32)
    image_classes = 10
    batch_size = 32
    lr = 0.01

    train_data = FakeDataSet(image_num, image_size, image_classes)
    train_loader = DataLoader(train_data, batch_size=batch_size)

    model = LeNet().to(device)
    criterion = nn.CrossEntropyLoss()

    optimizer = torch.optim.Adam(model.parameters(), lr=lr)
    train_one_epoch(train_loader, model, optimizer, criterion)


if __name__ == "__main__":
    LeNetTrain()
