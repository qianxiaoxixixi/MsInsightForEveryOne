import sys
import json
from validate_utils import validate_compare_db, get_query_result, CONTENT

file_path = sys.argv[1]

validate_compare_db(file_path, 69, 69, 'tensor')

def validate_json():
    try:
        query_result_n = get_query_result(file_path, "SELECT input_data, output_data FROM tb_nodes WHERE data_source='NPU' AND node_name ='Functional.linear.0.forward'")
        node = query_result_n[0]
        out_data = json.loads(node[1])
        in_data = json.loads(node[0])
        if out_data.get('Functional.linear.0.forward.output.0').get('Cosine') != "1.0":
            print(CONTENT, 'Functional.linear.0.forward.output.0 Cosine != 1.0')
            exit()
        if in_data.get('Functional.linear.0.forward.input.2').get('Cosine') != "unsupported":
            print(CONTENT, 'Functional.linear.0.forward.input.2 Cosine != unsupported')
            exit()
    except Exception as e:
        print(CONTENT, e)
        exit()


validate_json()
print("Validate the .vis.db file successfully.")
