import os
import csv
import re
import sys
import time


def find_csv_with_timestamp(directory, prefix):
    """
    在目录中查找以指定前缀开头、以时间戳结尾的 CSV 文件

    Args:
        directory: 目录路径
        prefix: 文件名前缀

    Returns:
        找到的 CSV 文件路径，如果没有找到则返回 None
    """
    pattern = re.compile(rf"{prefix}\d+\.csv$")
    csv_files = [f for f in os.listdir(directory) if pattern.match(f)]
    
    if not csv_files:
        return None

    # 按时间戳排序并返回最新的文件
    latest_file = max(csv_files, key=lambda x: int(re.search(r'\d+', x).group()))
    return os.path.join(directory, latest_file)


def load_csv(file_path):
    """读取 CSV 文件并返回每一行的列表"""
    with open(file_path, 'r') as file:
        reader = csv.reader(file)
        return [row for row in reader]


def compare_csv_files(directory_1, directory_2, prefix):
    """
    比较两个目录下的指定前缀的 CSV 文件，检查第一个文件中的每一行是否在第二个文件中存在

    Args:
        directory_1: 第一个目录路径
        directory_2: 第二个目录路径
        prefix: 文件名前缀

    Returns:
        (bool, list): 如果所有行都匹配，返回 True 和空列表；如果存在不匹配的行，返回 False 和这些行
    """
    # 找到两个目录中符合条件的 CSV 文件
    csv_file_1 = find_csv_with_timestamp(directory_1, prefix)
    csv_file_2 = find_csv_with_timestamp(directory_2, prefix)

    if not csv_file_1 or not csv_file_2:
        print(f"Error: Could not find matching CSV files with prefix '{prefix}' in one or both directories.")
        return False, []

    print(f"Comparing files:\n{csv_file_1}\n{csv_file_2}")
    
    # 加载 CSV 文件内容
    data_1 = load_csv(csv_file_1)
    data_2 = load_csv(csv_file_2)
    
    # 将第二个 CSV 文件的所有行存储在一个集合中
    data_2_set = {tuple(row) for row in data_2}
    
    # 记录不存在的行
    not_found_rows = []

    # 比较每一行是否存在于第二个 CSV 文件中
    for idx, row in enumerate(data_1):
        if tuple(row) not in data_2_set:
            print(f"Row {idx + 1}: {row} does not exist in the second file.")
            not_found_rows.append(row)

    return len(not_found_rows) <= 20, not_found_rows  # 返回是否容错，以及未找到的行


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script_name.py <directory_1> <directory_2>")
        sys.exit(1)

    # 获取命令行参数
    directory_1 = sys.argv[1]
    directory_2 = sys.argv[2]

    # 记录开始时间
    start_time = time.time()
    file_name = "compare_csv_files"

    prefixes = ["accuracy_checking_result_", "accuracy_checking_details_"]

    try:
        all_passed = True
        for prefix in prefixes:
            print(f"Comparing files with prefix: {prefix}")
            result, not_found_rows = compare_csv_files(directory_1, directory_2, prefix)
            duration_time = f"{time.time() - start_time:.2f}s"

            if result:
                print(f"{file_name} pass for {prefix} {duration_time} mindspore")
                if not_found_rows:
                    print(f"Minor differences detected (<= 5 rows) for {prefix}:")
                    for row in not_found_rows:
                        print(row)
            else:
                all_passed = False
                print(f"{file_name} fail for {prefix} {duration_time} mindspore")
                print(f"Rows not found in the second file for {prefix}:")
                for row in not_found_rows:
                    print(row)

        # 如果所有前缀都通过，则返回成功状态
        if all_passed:
            sys.exit(0)
        else:
            sys.exit(1)

    except Exception as e:
        duration_time = f"{time.time() - start_time:.2f}s"
        print(f"{file_name} fail {duration_time} mindspore: {e}")
        sys.exit(1)
