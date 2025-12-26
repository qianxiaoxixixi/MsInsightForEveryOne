import os

import numpy as np

def compare_kbyk_output(base_path, real_path):
    if not os.path.exists(base_path) or not os.path.exists(real_path):
        return False

    base_path_length = len(base_path)
    real_path_length = len(real_path)
    base_files = []
    real_files = []
    for root, _, files in os.walk(base_path):
        for file in files:
            base_files.append(os.path.join(root, file))
    for root, _, files in os.walk(real_path):
        for file in files:
            real_files.append(os.path.join(root, file))

    if len(base_files) != len(real_files):
        return False

    for base_file in base_files:
        real_file = base_file[base_path_length:]
        if real_file[0] == '/' or real_file[0] == '\\':
            real_file = real_file[1:]
        real_file = os.path.join(real_path, real_file)
        if real_file not in real_files:
            return False
        if base_file[-4:] == ".npy":
            value1 = np.load(base_file)
            value2 = np.load(real_file)
            if value1.shape != value2.shape or value1.dtype != value2.dtype:
                return False
            if not (value1 == value2).all():
                return False
    return True



if compare_kbyk_output('./baseline/test_ms_set_dump_cell_L1', './baseline/test_ms_set_dump_op_L1'):
    print("pass")
else:
    print("fail")
