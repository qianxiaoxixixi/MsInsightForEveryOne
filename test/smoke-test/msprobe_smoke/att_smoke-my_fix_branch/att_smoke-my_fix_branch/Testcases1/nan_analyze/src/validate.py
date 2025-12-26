import sys
import json

if __name__ == '__main__':
    args = sys.argv
    if len(args) != 3:
        print('Unexpected number of args! Usage: \n    python validate.py {anomaly_type} {file_path}')
        exit(1)
    anomaly_type = args[1]
    file_path = args[2]
    with open(file_path, 'r') as f:
        data = json.load(f)
        if anomaly_type == 'pre':
            if data.get('rank_0') and data.get('rank_2'):
                flag_rank0 = len(data['rank_0']) == 1 and data['rank_0'][0]['op_name'] == 'Functional.pad.2.forward'
                flag_rank2 = len(data['rank_2']) == 1 and data['rank_2'][0]['op_name'] == 'Functional.pad.2.forward'
                if flag_rank0 and flag_rank2:
                    print('Validate the json file successfully.')
                    exit(0)
            print('Validate the json file failed.')
            exit(1)
        elif anomaly_type == 'mid':
            if data.get('rank_0'):
                flag_rank0 = len(data['rank_0']) == 1 and data['rank_0'][0]['op_name'] == 'Tensor.unsqueeze.0.forward'
                if flag_rank0:
                    print('Validate the json file successfully.')
                    exit(0)
            print('Validate the json file failed.')
            exit(1)
        elif anomaly_type == 'post':
            if data.get('rank_12') and data.get('rank_15'):
                flag_rank12 = len(data['rank_12']) == 1 and data['rank_12'][0]['op_name'] == 'Tensor.__add__.798.forward'
                flag_rank15 = len(data['rank_15']) == 1 and data['rank_15'][0]['op_name'] == 'Tensor.__radd__.2.forward'
                if flag_rank12 and flag_rank15:
                    print('Validate the json file successfully.')
                    exit(0)
            print('Validate the json file failed.')
            exit(1)
        else:
            print(f'Unexpected type arg: {anomaly_type}!')
            exit(1)

