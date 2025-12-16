import sys
import json
from validate_utils import validate_compare_db, get_query_result, CONTENT

file_path = sys.argv[1]

validate_compare_db(file_path, 2851, 2824, 'md5')

def validate_json(default_pass_num_n):
    try:
        pass_num_n = 0
        query_result_n = get_query_result(file_path, "SELECT precision_index FROM tb_nodes WHERE data_source='NPU' AND node_name !='DefaultModel'")
        for item in query_result_n:
            if item[0] == 1:
                pass_num_n += 1
        if pass_num_n != default_pass_num_n:
            print(CONTENT, f"The number of pass n_nodes is {pass_num_n}, and the expected number is {default_pass_num_n}.")
            exit()
    except Exception as e:
        print(CONTENT, e)
        exit()


validate_json(825)
print("Validate the .vis.db file successfully.")
