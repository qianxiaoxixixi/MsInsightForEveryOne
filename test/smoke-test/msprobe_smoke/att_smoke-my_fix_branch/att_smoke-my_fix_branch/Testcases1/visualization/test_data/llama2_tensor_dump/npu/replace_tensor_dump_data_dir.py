import json
import os

current_path = os.getcwd()

filename = os.path.join(current_path, 'dump.json')

with open(filename, 'r') as file:
    data = json.load(file)

new_dir = os.path.join(current_path, 'dump_tensor_data')
data['dump_data_dir'] = new_dir

with open(filename, 'w') as file:
    json.dump(data, file, indent=4)

print(f"Updated JSON file dump_data_dir with the {new_dir}.")
