import sys
import json
from validate_utils import validate_compare_db, get_query_result, CONTENT

file_path = sys.argv[1]

validate_compare_db(file_path, compare_mode='tensor')

def validate_stack(stack_info, node_name, stack_content):
    try:
        if stack_content not in json.dumps(stack_info):
            print(CONTENT, f'{node_name} stack info error.')
            exit() 
    except Exception as e:
        print(CONTENT, e)
        exit()

def validate_json():
    node_name = 'Module.conv1.Conv2d.forward.0'
    try:
        query_result_n = get_query_result(file_path, f"SELECT b.stack_info FROM tb_nodes a JOIN tb_stack b ON a.stack_id = b.id WHERE a.data_source='NPU' AND a.node_name ='{node_name}'")
        stack_n = json.loads(query_result_n[0][0])
        query_result_b = get_query_result(file_path, f"SELECT b.stack_info FROM tb_nodes a JOIN tb_stack b ON a.stack_id = b.id WHERE a.data_source='Bench' AND a.node_name ='{node_name}'")
        stack_b = json.loads(query_result_b[0][0])
    except Exception as e:
        print(CONTENT, e)
        exit()
    validate_stack(stack_n, node_name, 'x = self.conv1(x)')
    validate_stack(stack_b, node_name, 'x = self.conv1(x)')

def validate_json1():
    node_name = 'Functional.conv2d.0.forward'
    try:
        query_result_n = get_query_result(file_path, f"SELECT b.stack_info FROM tb_nodes a JOIN tb_stack b ON a.stack_id = b.id WHERE a.data_source='NPU' AND a.node_name ='{node_name}'")
        stack_n = json.loads(query_result_n[0][0])
        query_result_b = get_query_result(file_path, f"SELECT b.stack_info FROM tb_nodes a JOIN tb_stack b ON a.stack_id = b.id WHERE a.data_source='Bench' AND a.node_name ='{node_name}'")
        stack_b = json.loads(query_result_b[0][0])
    except Exception as e:
        print(CONTENT, e)
        exit()
    validate_stack(stack_n, node_name, 'return F.conv2d(')
    validate_stack(stack_b, node_name, 'return F.conv2d(')

validate_json()
validate_json1()
print("Validate the .vis.db file successfully.")
