import sys
import torch
import torch_npu
from torch.utils.data import Dataset, DataLoader
from torch.utils.data.distributed import DistributedSampler
import torch.multiprocessing as mp
from torch.nn.parallel import DistributedDataParallel as DDP
from torch.distributed import init_process_group, destroy_process_group
import os
import torch.nn as nn 


class MyTrainDataset(Dataset):
    def __init__(self, size, shape=(20,)):
        super().__init__()
        self.data = torch.randn(size, *shape)
        self.label = torch.randn(size) * 10
        self.size = size 

    def __len__(self):
        return self.size 
    
    def __getitem__(self, idx):
        return self.data[idx], self.label[idx]

class TestNet(nn.Module):
    def __init__(self):
        super(TestNet, self).__init__()
        self.linear1 = nn.Linear(20, 10)
        self.linear2 = nn.Linear(10, 1)

    def forward(self, x):
        x = self.linear1(x)
        x = self.linear2(x)
        return x 

def prepare_dataloader(dataset: Dataset, batch_size: int, world_size: int):
    return DataLoader(
        dataset,
        batch_size=batch_size,
        pin_memory=True,
        shuffle=False if world_size != -1 else True,
        sampler=DistributedSampler(dataset) if world_size != -1 else None
    )

def main(rank, world_size, free_npu_info, config_filepath):
    torch.npu.set_device(f"npu:{free_npu_info[rank]}")
    if world_size != -1:
        os.environ['MASTER_ADDR'] = '127.0.0.1'
        os.environ['MASTER_PORT'] = '29689'
        os.environ['HCCL_WHITELIST_DISABLE'] = '1'
        init_process_group(backend='hccl', rank=rank, world_size=world_size)
    train_data = prepare_dataloader(MyTrainDataset(128), batch_size=32, world_size=world_size)
    model = TestNet()
    model.npu()
    if world_size != -1:
        model = DDP(model, device_ids=[free_npu_info[rank]], broadcast_buffers=False)
    model.npu()

    from msprobe.pytorch import PrecisionDebugger
    debugger = PrecisionDebugger(config_filepath)
    debugger.monitor(model)

    loss_fun = nn.CrossEntropyLoss()
    loss_fun.npu()

    learning_rate = 1e-2
    optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)

    for source, targets in train_data:
        print(source.shape, targets.shape)

        source = source.npu()
        targets = targets.npu().long()
        optimizer.zero_grad()
        output = model(source)
        loss = loss_fun(output, targets)
        loss.backward()
        optimizer.step()

    if world_size != -1:
        destroy_process_group()

def get_free_npu_info(number):
    import subprocess
    import re
    output = subprocess.check_output(["/usr/local/bin/npu-smi", "info"]).decode("utf-8")
    pattern = r"No running processes found in NPU (\d+)"
    matches = re.findall(pattern, output)
    if len(matches) < number:
        raise Exception(f"no enough npu, at least {number}")
    return [int(x) for x in matches[-number:]]

if __name__ == "__main__":
    config_filepath=sys.argv[1]
    n_device = int(sys.argv[2])
    free_npu_info = get_free_npu_info(n_device)

    if n_device >= 2:
        world_size = n_device
        mp.spawn(main, args=(world_size, free_npu_info, config_filepath), nprocs=world_size, join=True)
    else:
        device = 0
        main(0, -1, free_npu_info, config_filepath)
