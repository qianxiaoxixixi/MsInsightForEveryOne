import torch
import random


def convert_to_inf(item: dict):
    case = random.randint(0, 2)
    if case == 0:
        item['Max'] = torch.inf
        item['Mean'] = torch.inf
        item['Norm'] = torch.inf
    elif case == 1:
        item['Min'] = -torch.inf
        item['Mean'] = -torch.inf
        item['Norm'] = -torch.inf
    else:
        item['Max'] = torch.inf
        item['Min'] = -torch.inf
        item['Mean'] = torch.nan
        item['Norm'] = torch.nan


def convert_to_first_anomaly(item: dict):
    for i in item['output']:
        if isinstance(i, dict) and i.get('type') == 'torch.Tensor':
            convert_to_inf(i)


def convert_to_subsequent_anomaly(item: dict):
    for i in item['input_args']:
        if isinstance(i, dict) and i.get('type') == 'torch.Tensor':
            convert_to_inf(i)
    for _, kwarg in item['input_kwargs'].items():
        if isinstance(kwarg, dict) and kwarg.get('type') == 'torch.Tensor':
            convert_to_inf(kwarg)
    convert_to_first_anomaly(item)


if __name__ == '__main__':
    import sys
    args = sys.argv
    if len(args) < 2:
        exit(0)
    arg = args[1]
    base_path = args[2]
    tar_path = args[3]
    apis = {}
    if arg == 'pre':
        apis[0] = {'Functional.pad.2.forward': 0, 'Functional.pad.3.forward': 1, 'Distributed.broadcast.0.forward': 1}
        apis[1] = {'Distributed.broadcast.0.forward': 1, 'Distributed.broadcast.1.forward': 1}
        apis[2] = {'Functional.pad.2.forward': 0, 'Distributed.broadcast.0.forward': 1, 'Distributed.broadcast.1.forward': 1}
        apis[3] = {'Distributed.broadcast.0.forward': 1}
    elif arg == 'mid':
        apis[0] = {'Tensor.unsqueeze.0.forward': 0, 'Tensor.sum.5.forward': 1, 'Tensor.__add__.3.forward': 1, 'Distributed.isend.0.forward': 1}
        apis[4] = {'Distributed.irecv.0.forward': 1}
    elif arg == 'post':
        apis[12] = {'Tensor.__add__.798.forward': 0, 'Tensor.__truediv__.608.forward': 1}
        apis[15] = {'Tensor.__radd__.2.forward': 0, 'Tensor.__truediv__.608.forward': 1}
        apis[15].update({f'Tensor.__add__.{i}.forward': 1 for i in range(784, 799)})
    else:
        exit(0)
    import json
    import os
    import shutil
    os.makedirs(tar_path, exist_ok=True)
    for rank_path in os.listdir(base_path):
        os.mkdir(f'{tar_path}/{rank_path}')
        shutil.copyfile(f'{base_path}/{rank_path}/construct.json', f'{tar_path}/{rank_path}/construct.json')
        shutil.copyfile(f'{base_path}/{rank_path}/stack.json', f'{tar_path}/{rank_path}/stack.json')
        shutil.copyfile(f'{base_path}/{rank_path}/dump.json', f'{tar_path}/{rank_path}/dump.json')
    for rank, api_dict in apis.items():
        with open(f'{base_path}/rank{rank}/dump.json') as f:
            j = json.load(f)
            data = j['data']
            for api, is_subsequent in api_dict.items():
                try:
                    if is_subsequent:
                        convert_to_subsequent_anomaly(data[api])
                    else:
                        convert_to_first_anomaly(data[api])
                except Exception as e:
                    print('api:', api)
                    print(data[api])
                    raise e
        with open(f'{tar_path}/rank{rank}/dump.json', 'w') as tar:
            json.dump(j, tar, indent=2)

