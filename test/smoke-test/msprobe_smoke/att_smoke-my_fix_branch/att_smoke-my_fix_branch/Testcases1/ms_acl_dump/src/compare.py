import os
import sys


def should_dive(path):
    """判断最后一级目录名是否以数字开头，不以数字开头则需要向下2层"""
    base_name = os.path.basename(os.path.normpath(path))
    return not base_name or not base_name[0].isdigit()

def dive_two_levels(path):
    """向下进入两层子目录"""
    current = path
    for _ in range(2):
        subdirs = [d for d in os.listdir(current) if os.path.isdir(os.path.join(current, d))]
        if not subdirs:
            break
        subdirs.sort()
        current = os.path.join(current, subdirs[0])
    return current

def compare_acl_bin(base_path, real_path):
    if not os.path.exists(base_path) or not os.path.exists(real_path):
        return False

    if should_dive(base_path):
        base_path = dive_two_levels(base_path)
    if should_dive(real_path):
        real_path = dive_two_levels(real_path)

    base_dirs = []
    base_files = []
    real_dirs = []
    real_files = []
    for _, dirs, files in os.walk(base_path):
        base_dirs.append(dirs)
        for file in files:
            base_files.append(file)
    for _, dirs, files in os.walk(real_path):
        real_dirs.append(dirs)
        for file in files:
            real_files.append(file)

    if len(base_files) != len(real_files):
        print("The number of files is not equal")
        return False

    if len(base_dirs) != len(real_dirs):
        print("The construction of directory is not equal")
        return False

    for i in range(len(base_dirs)):
        if len(base_dirs[i]) != len(real_dirs[i]):
            print("The construction of directory is not equal")
            return False
        for j in range(len(base_dirs[i])):
            if base_dirs[i][j].isdigit():
                continue
            if base_dirs[i][j] != real_dirs[i][j]:
                print("The construction of directory is not equal")
                return False

    return True


def compare_acl_stat(base_path, real_path):
    if should_dive(base_path):
        base_path = dive_two_levels(base_path)
    if should_dive(real_path):
        real_path = dive_two_levels(real_path)

    if not compare_acl_bin(base_path, real_path):
        return False

    base_files = []
    real_files = []

    for root, _, files in os.walk(base_path):
        for file in files:
            if ".csv" in file and file != "mapping.csv":
                base_file = os.path.join(root, file)
                base_files.append(base_file)

    for root, _, files in os.walk(real_path):
        for file in files:
            if ".csv" in file and file != "mapping.csv":
                real_file = os.path.join(root, file)
                real_files.append(real_file)

    if not real_file:
        print("no csv file")
        return False

    for base_file in base_files:
        base_file_segs = base_file.split('/')
        real_file = ''
        for file in real_files:
            file_segs = file.split('/')
            if file_segs[-4] == "Net":
                if file_segs[-4] == base_file_segs[-4] and file_segs[-2] == base_file_segs[-2] and \
                  file_segs[-1] == base_file_segs[-1] and file_segs[-3] == base_file_segs[-3]:
                    real_file = file
                    break
            else:
                if file_segs[-4] == base_file_segs[-4] and file_segs[-2] == base_file_segs[-2] and \
                  file_segs[-1] == base_file_segs[-1]:
                    real_file = file
                    break
        if not real_file:
            print(f'missing file corresponding to {base_file}.')
            return False

        with open(base_file, 'r') as f:
            base_file_content = f.read()
        with open(real_file, 'r') as f:
            real_file_content = f.read()
        base_file_content_segs = base_file_content.split('\n')
        real_file_content_segs = real_file_content.split('\n')
        if len(real_file_content_segs) != len(base_file_content_segs) or real_file_content_segs[0] != base_file_content_segs[0]:
            print(f'{real_file} and {base_file} is different.')
            return False
    
    return True

if __name__ == '__main__':
    if len(sys.argv) == 4:
        if sys.argv[3] == "bin":
            if compare_acl_bin(sys.argv[1], sys.argv[2]):
                print("pass")
            else:
                print("fail")
        elif sys.argv[3] == "stat":
            if compare_acl_stat(sys.argv[1], sys.argv[2]):
                print("pass")
            else:
                print("fail")
    else:
        print("params error")
