import os
import sys

import numpy as np


def compare_kbyk_output(base_path, real_path):
    if not os.path.exists(base_path) or not os.path.exists(real_path):
        return False

    base_path_length = len(base_path)
    base_files = []
    real_files = []
    for root, _, files in os.walk(base_path):
        for file in files:
            base_files.append(os.path.join(root, file))
    for root, _, files in os.walk(real_path):
        for file in files:
            real_files.append(os.path.join(root, file))

    if len(base_files) != len(real_files):
        print("The number of files is not equal")
        return False

    for base_file in base_files:
        real_file = base_file[base_path_length:]
        if real_file[0] == '/' or real_file[0] == '\\':
            real_file = real_file[1:]
        real_file = os.path.join(real_path, real_file)
        if len(base_file)>= 4 and base_file[-4:] == ".npy":
            real_file_list = real_file.split('.')
            npy_find = False
            for file in real_files:
                if len(file)>= 4 and file[-4:] == ".npy":
                    file_list = file.split('.')
                    file_list[-5] = real_file_list[-5]
                    if (file_list == real_file_list):
                        real_file = file
                        npy_find = True
                        break
            if not npy_find:
                print(f"real_file: {real_file.replace('pass', '').replace('fail', '')} does not exist")
                return False
            value1 = np.load(base_file)
            value2 = np.load(real_file)
            if value1.shape != value2.shape or value1.dtype != value2.dtype:
                print(f"npy: {real_file.replace('pass', '').replace('fail', '')} has different shape or dtype")
                return False
            if not (value1 == value2).all():
                print(f"npy: {real_file.replace('pass', '').replace('fail', '')} has different value")
                return False
        else:
            if real_file not in real_files:
                print(f"real_file: {real_file.replace('pass', '').replace('fail', '')} does not exist")
                return False
    return True


if __name__ == '__main__':
    if len(sys.argv) == 3:
        if compare_kbyk_output(sys.argv[1], sys.argv[2]):
            print("pass")
        else:
            print("fail")
    else:
        print("params error")
