
import sys
import os
import csv


def get_args():
    if len(sys.argv) != 5:
        raise ValueError
    level = int(sys.argv[1])
    if level not in [0, 1, 2]:
        raise ValueError
    mode = sys.argv[2]
    if mode not in ["pynative", "graph"]:
        raise ValueError
    csv_path = sys.argv[3]
    if not os.path.isfile(csv_path):
        raise ValueError
    row_num = int(sys.argv[4])

    return level, mode, csv_path, row_num

column_mapping = {
    (0, "pynative"): ["param_name", "MD5", "max", "min", "norm", "shape"],
    (1, "pynative"): ["param_name", "max", "min", "norm", "shape"],
    (2, "pynative"): ["param_name", "(-inf, -1.0]", "(-1.0, 0.0]", "(0.0, 1.0]", "(1.0, inf)", "=0", "max", "min", "norm", "shape"],
    (0, "graph"): ["Param_name", "Max", "Min", "Norm", "Shape"],
    (1, "graph"): ["Param_name", "Max", "Min", "Norm", "Shape"],
    (2, "graph"): ["Param_name", "(-inf, -1.0]", "(-1.0, 0.0]", "(0.0, 1.0]", "(1.0, inf)", "=0", "Max", "Min", "Norm", "Shape"],

}


def check_grad_summary_csv(level, mode, csv_path, row_num):
    column_entries = column_mapping[(level, mode)]
    rows = []
    with open(csv_path, mode='r', encoding='utf-8-sig') as file:
        csv_reader = csv.reader(file)
        for row in csv_reader:
            rows.append(row)
    if len(rows) != row_num:
        print(f"wrong row number, target: {row_num}, real: {len(rows)}")
        return False
    else:
        print("row num check pass")
    if column_entries != rows[0]:
        print(f"wrong column, target column: {column_entries}, real column {rows[0]}")
        return False
    else:
        print("column check pass")
    return True



if __name__ == "__main__":
    level, mode, csv_path, row_num = get_args()
    print(f"starting checking csv: {csv_path}")
    is_pass = check_grad_summary_csv(level, mode, csv_path, row_num)
    if is_pass:
        sys.exit(0)
    else:
        sys.exit(1)





