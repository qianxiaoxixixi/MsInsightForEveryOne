import sys
import json
from validate_utils import validate_compare_db, get_query_result, CONTENT

file_path = sys.argv[1]

def validate_json():
    try:
        query_result_n = get_query_result(file_path, "SELECT input_data, output_data, matched_node_link FROM tb_nodes WHERE data_source='NPU' AND node_name ='Module.layer1.Linear.forward.0'")
        node = query_result_n[0]
        out_data = json.loads(node[1])
        in_data = json.loads(node[0])
        if out_data.get('Module.layer1.Linear.forward.0.output.0').get('Max diff', "null") != "0.0":
            print(CONTENT, 'Module.layer1.Linear.forward.0.output.0 Max diff != 0.0')
            exit()
        if in_data.get('Module.layer1.Linear.forward.0.input.0').get('MaxRelativeErr', "null") != "0.0%":
            print(CONTENT, 'Module.layer1.Linear.forward.0.input.0 MaxRelativeErr != 0.0%')
            exit()
        matched_node_link = json.loads(node[2])
        if matched_node_link != ["DefaultModel", "Module.layers1.Linear.forward.0"]:
            print(CONTENT, f'matched_node_link error, current info: {matched_node_link}, expect ["DefaultModel", "Module.layers1.Linear.forward.0"]')
            exit()
    except Exception as e:
        print(CONTENT, e)
        exit()

validate_json()
print("Validate the .vis.db file successfully.")
