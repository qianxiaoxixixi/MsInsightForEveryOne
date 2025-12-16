import sqlite3
import os
import json

CONTENT = "[ERROR]: The content of the .vis.db file is incorrect."


def execute_query(db_path, query):
    if not os.path.exists(db_path):
        print(CONTENT, 'not exist!')
        return None

    try:
        conn = sqlite3.connect(db_path)
        cursor = conn.cursor()

        cursor.execute(query)

        results = cursor.fetchall()
        return results

    except sqlite3.Error as e:
        print(f"{CONTENT}: {e}")
    finally:
        if conn:
            conn.close()

def get_query_result(db_path, query):
    query_result = execute_query(db_path, query)
    if query_result is None:
        exit()
    return query_result

def validate_build_db(db_path, default_nodes_num, data_source='NPU', step=0, rank=0, use_compare=False):

    query_result = get_query_result(db_path, f"SELECT COUNT(*) FROM tb_nodes WHERE data_source='{data_source}'")
    nodes_num = query_result[0][0]
    if nodes_num != default_nodes_num:
        print(CONTENT, f"The number of nodes is {nodes_num}, and the expected number is {default_nodes_num}.")
        exit()
    
    query_result = get_query_result(db_path, f"SELECT id FROM tb_nodes WHERE step={step} AND rank={rank} AND data_source='{data_source}' AND node_name='DefaultModel'")
    if len(query_result) != 1:
        print(CONTENT, f"The num of node 'DefaultModel' error, query_result is {len(query_result)} and the expected result is 1.")
        exit()

    if not use_compare:
        validate_node_in_out_info_build(db_path)


def validate_node_in_out_info_build(db_path, step=0, rank=0):

    query_result = get_query_result(db_path, f"SELECT input_data, output_data, matched_node_link FROM tb_nodes WHERE step={step} AND rank={rank} AND node_name!='DefaultModel'")

    validate_node_in_out_info(query_result, False, ["dtype", "shape", "Max", "Min", "Mean", "Norm"], [])


def validate_compare_db(db_path, default_nodes_num_n=None, default_nodes_num_b=None, compare_mode='statistics', step=0, rank=0):
    ALL_KEY = []
    if compare_mode == 'statistics':
        ALL_KEY = ["dtype", "shape", "Max", "Min", "Mean", "Norm", "Max diff", "Min diff", "Mean diff", "L2norm diff",
            "MaxRelativeErr", "MinRelativeErr", "MeanRelativeErr", "NormRelativeErr"]
    elif compare_mode == 'tensor':
        ALL_KEY = ["dtype", "shape", "Max", "Min", "Mean", "Norm", "Cosine", "MaxAbsErr", "MaxRelativeErr",
            "One Thousandth Err Ratio", "Five Thousandths Err Ratio"]
    KEY = ["dtype", "shape", "Max", "Min", "Mean", "Norm"]
    if default_nodes_num_n is not None and default_nodes_num_b is not None:
        validate_build_db(db_path, default_nodes_num_n, use_compare=True)
        validate_build_db(db_path, default_nodes_num_b, data_source='Bench', use_compare=True)
    if compare_mode in ['statistics', 'tensor']:
        query_result_n = get_query_result(db_path, f"SELECT input_data, output_data, matched_node_link FROM tb_nodes WHERE step={step} AND rank={rank} AND data_source='NPU' AND node_name!='DefaultModel'")
        validate_node_in_out_info(query_result_n, True, KEY, ALL_KEY)
        query_result_b = get_query_result(db_path, f"SELECT input_data, output_data, matched_node_link FROM tb_nodes WHERE step={step} AND rank={rank} AND data_source='Bench' AND node_name!='DefaultModel'")
        validate_node_in_out_info(query_result_b, False, KEY, ALL_KEY)


def validate_data_index(data: dict, is_compare_npu, is_matched, key_list, all_key_list):
    for key, value in data.items():
        if not value or not isinstance(value, dict):
            print(CONTENT)
            exit()
        if len(value) == 1:
            if value.get("value") != "null":
                print(CONTENT, f"the '{key}' content is {value.get('value')}, and the expected content is null.")
                exit()
        elif is_matched and is_compare_npu:
            for item in all_key_list:
                if value.get('type') not in ['torch.Tensor', 'int', 'float', 'bool']:
                    continue
                if value.get(item) is None:
                    print(CONTENT, f"{item} in '{key}' is missing.")
                    exit()
        else:
            for item in key_list:
                if value.get('type') not in ['torch.Tensor', 'int', 'float', 'bool']:
                    continue
                if value.get(item) is None:
                    print(CONTENT, f"{item} in '{key}' is missing.")
                    exit()


def validate_node_in_out_info(in_out_result, is_compare_npu, key_list, all_key_list):
    # in_out_result: [(in1, out1, matched), (in2, out2, matched), (in3, out3, matched) ...]
    try:
        for item in in_out_result:
            is_matched = item[2] != '[]'
            validate_data_index(json.loads(item[0]), is_compare_npu, is_matched, key_list, all_key_list)
            validate_data_index(json.loads(item[1]), is_compare_npu, is_matched, key_list, all_key_list)
    except Exception as e:
        print(f"{CONTENT}: {e}")
        exit()

def validate_one_node_matched_distributed(file_path, node_id, matched_distributed_template, step=0, rank=0):
    sql = f"SELECT matched_distributed FROM tb_nodes WHERE step={step} AND rank={rank} AND node_name='{node_id}'"
    result = get_query_result(file_path, sql)
    if not result:
        print(CONTENT, f"cannot found {node_id}!")
        exit()
    if json.loads(result[0][0]) != matched_distributed_template:
        print(CONTENT, f"the 'matched_distributed' content in {node_id} is incorrect!")
        exit()

def check_dist_matched_num(file_path, dist_matched_num, step=0, rank=0):
    matched_num_result = get_query_result(file_path, f"SELECT COUNT(*) FROM tb_nodes WHERE step={step} AND rank={rank} AND matched_distributed != '{{}}'")
    current_dist_matched_num = matched_num_result[0][0]

    if current_dist_matched_num != dist_matched_num:
        print(CONTENT, f"The number of dist_matched_num is {current_dist_matched_num}, and the expected number is {dist_matched_num}.")
        exit()
