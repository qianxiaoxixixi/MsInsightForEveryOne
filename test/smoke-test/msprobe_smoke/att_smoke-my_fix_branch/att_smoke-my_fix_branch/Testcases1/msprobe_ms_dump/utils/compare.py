import json
import numpy as np
import pandas as pd
from scipy.spatial.distance import cosine
import sys
import os

def load_json(file_path):
    with open(file_path, 'r') as f:
        return json.load(f)

def calculate_relative_error(value1, value2):
    difference = abs(value1 - value2)

    # 如果差异小于或等于 0.1，返回 0
    if difference <= 0.1:
        return 0

    # 否则，返回相对误差
    return abs(value1 - value2) / abs(value1) if value1 != 0 else np.inf

def calculate_metrics(data1, data2):
    metrics = []
    
    # 记录缺失的键
    missing_keys = []
    for key in data1.keys():
        if "empty" in key:
            continue
        if key in data2:
            item1 = data1[key]
            item2 = data2[key]

            try:
                for idx, (arg1, arg2) in enumerate(zip(item1['output'], item2['output'])):
                    if arg1 and arg2 and 'Mean' in arg1 and 'Mean' in arg2:
                        mean1 = arg1['Mean']
                        mean2 = arg2['Mean']
                        
                        if mean1 is not None and mean2 is not None:
                            # Calculate relative error
                            rel_error = calculate_relative_error(mean1, mean2)
                            
                            # Calculate cosine similarity
                            vector1 = np.array([mean1, arg1['Max'], arg1['Min'], arg1['Norm']])
                            vector2 = np.array([mean2, arg2['Max'], arg2['Min'], arg2['Norm']])
                            cos_sim = 1 - cosine(vector1, vector2)
                            
                            metrics.append([key, rel_error, cos_sim])
            except Exception as e:
                print(f"[ERROR] Failed to process key '{key}': {e}")
                print(f"item1: {item1}")
                print(f"item2: {item2}")
        else:
            metrics.append([key, 123, 123])
            # 如果 key 不在 data2 中，记录缺失的算子
            missing_keys.append(key)

    if missing_keys:
        # 如果存在缺失的键，打印错误并返回
        print(f"[ERROR] The following operators are present in the first file but missing in the second: {missing_keys}")
        # sys.exit(1) 

    return metrics

def generate_comparison_table(metrics):
    df = pd.DataFrame(metrics, columns=['Operator', 'Relative Error', 'Cosine Similarity'])
    return df

def main():
    if len(sys.argv) != 4:
        print("Usage: python compare.py <json_file1> <json_file2> <output_dir>")
        sys.exit(1)

    json_file1 = sys.argv[1]
    json_file2 = sys.argv[2]
    output_dir = sys.argv[3]

    # Load JSON files
    data1 = load_json(json_file1)
    data2 = load_json(json_file2)

    # Calculate metrics
    metrics = calculate_metrics(data1['data'], data2['data'])

    # Generate table
    comparison_table = generate_comparison_table(metrics)

    # Ensure output directory exists
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    # Save the table to the specified output directory
    output_file = os.path.join(output_dir, 'comparison_table.csv')
    comparison_table.to_csv(output_file, index=False)
    print(f"Comparison table saved to {output_file}")

if __name__ == "__main__":
    main()
