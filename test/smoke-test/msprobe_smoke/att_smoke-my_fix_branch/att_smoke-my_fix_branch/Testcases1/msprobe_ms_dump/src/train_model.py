import mindspore as ms
import mindspore.dataset as ds
import mindspore.dataset.vision as vision
import mindspore.dataset.transforms as transforms
import mindspore.nn as nn
from mindspore import dtype as mstype
from mindspore.communication import init, get_rank, get_group_size
from mindspore.train import Model
from mindspore.train import LossMonitor
from resnet import resnet50
import sys
from msprobe.mindspore.common.utils import MsprobeStep
from msprobe.mindspore import PrecisionDebugger
#seed_all(mode=True)

config_json = sys.argv[1]
debugger = PrecisionDebugger(config_path = config_json)

ms.set_context(mode=ms.PYNATIVE_MODE, device_target="Ascend")
ms.set_auto_parallel_context(parallel_mode=ms.ParallelMode.DATA_PARALLEL, gradients_mean=True)
init()
ms.set_seed(1)

data_dir = "/data00/Dataset/cifar-10-batches-bin"
batch_size = 256
image_size = 32
workers = 4
num_classes = 10

def create_dataset_cifar10(dataset_dir, usage, resize, batch_size, workers):
    data_set = ds.Cifar10Dataset(dataset_dir=data_dir, usage=usage, shuffle=True, num_shards=get_group_size(), shard_id=get_rank()) 
    
    trans = []
    if usage == "train":
        trans += [
            vision.RandomCrop((32, 32), (4, 4, 4, 4)),
            vision.RandomHorizontalFlip(prob=0.5)
        ]

    trans += [
        vision.Resize(resize),
        vision.Rescale(1.0 / 255.0, 0.0),
        vision.Normalize([0.4914, 0.4822, 0.4465], [0.2023, 0.1994, 0.2010]),
        vision.HWC2CHW()
    ]

    target_trans = transforms.TypeCast(mstype.int32)

    # image
    data_set = data_set.map(operations=trans,
                            input_columns='image',
                            num_parallel_workers=workers)

    data_set = data_set.map(operations=target_trans,
                            input_columns='label',
                            num_parallel_workers=workers)

    # label
    data_set = data_set.batch(batch_size)

    return data_set

dataset_train = create_dataset_cifar10(dataset_dir=data_dir,
                                       usage="test",
                                       resize=image_size,
                                       batch_size=batch_size,
                                       workers=workers)
step_size_train = dataset_train.get_dataset_size()

num_epochs = 1
lr = nn.cosine_decay_lr(min_lr=0.00001, max_lr=0.001, total_step=step_size_train * num_epochs,
                        step_per_epoch=step_size_train, decay_epoch=num_epochs)

net = resnet50()
opt = nn.Momentum(params=net.trainable_params(), learning_rate=lr, momentum=0.9)
loss_fn = nn.SoftmaxCrossEntropyWithLogits(sparse=True, reduction='sum')


trainer = Model(net, loss_fn=loss_fn, optimizer=opt, metrics={'accuracy'})
loss_monitor = LossMonitor(10)
debugger.start(net)
trainer.train(1, dataset_train, callbacks=[loss_monitor, MsprobeStep(debugger)])
print("TRAIN END")
#debugger.stop()
