import json
import os

# 标准的 JSON 模板
template = {
    "task": "statistics",
    "dump_path": "/data00/y30047314/maoyanlongbak/maoyan/dumpoutput/",
    "rank": [],
    "step": [],
    "level": "L0",
    "seed": 1234,
    "is_deterministic": False,
    "enable_dataloader": False,
    "acl_config": "",
    "tensor": {
        "scope": [],
        "list": [],
        "data_mode": ["all"],
        "backward_input": [],
        "file_format": "npy"
    },
    "statistics": {
        "scope": [],
        "list": [],
        "data_mode": ["all"],
        "summary_mode": "statistics"
    },
    "overflow_check": {
        "overflow_nums": 1,
        "check_mode": "all"
    },
    "run_ut": {
        "white_list": [],
        "black_list": [],
        "error_data_path": "./"
    }
}

# 配置选项

level = "L0"
tasks = ["statistics","tensor"]
ranks = [[],[0,1]]

# steps = [[], [0, 1], [2]]
steps = [[], [2]]
scopes = [[], ["Cell.relu.ReLU.forward.0", "Cell.relu.ReLU.backward.0"]]
# scopes = [[]]
# lists = [[], ["transpose"], ["cast"]]
lists = [[], ["ReLU"]]
# lists = [[]]
data_modes = [["all"],["input"],["output"]]

# 检查和创建输出目录
output_dir = "./json_configs"
if not os.path.exists(output_dir):
    os.makedirs(output_dir)

# 保存文件名的列表
run_msprobe_ms_dump_files = {
    1: [],
    2: [],
    3: [],
    4: []
}

# 生成组合并写入JSON文件
config_id = 0
for task in tasks:
    for step in steps:
        for rank in ranks:
            for data_mode in data_modes:
                if task == "statistics":
                    for scope in scopes:
                        for lst in lists:
                            if not (scope and lst):  # scope和list不能同时非空
                                config = template.copy()
                                config["task"] = task
                                config["rank"] = rank
                                config["step"] = step
                                config["statistics"]["scope"] = scope
                                config["statistics"]["list"] = lst
                                config["statistics"]["data_mode"] = data_mode

                                # 构建文件名
                                file_name_parts = [
                                    f"{level}",
                                    f"task_{task}",
                                    f"step_{'_'.join(map(str, step)) or 'all'}",
                                    f"rank_{'_'.join(map(str, rank)) or 'all'}",
                                    f"scope_{'_'.join(scope) or 'all'}",
                                    f"list_{'_'.join(lst) or 'all'}",
                                    f"data_mode_{'_'.join(data_mode)}"
                                ]
                                config_filename = "_".join(file_name_parts) + ".json"

                                with open(os.path.join(output_dir, config_filename), 'w') as f:
                                    json.dump(config, f, indent=4)

                                run_msprobe_ms_dump_files[3].append(config_filename.replace(".json", ".sh"))
                                config_id += 1
                elif task == "tensor":
                    for scope in scopes:
                        for lst in lists:
                            if not (scope and lst):  # list需要为空
                                config = template.copy()
                                config["task"] = task
                                config["rank"] = rank
                                config["step"] = step
                                config["tensor"]["scope"] = scope
                                config["tensor"]["list"] = lst
                                config["tensor"]["data_mode"] = data_mode

                                # 构建文件名
                                file_name_parts = [
                                    f"{level}",
                                    f"task_{task}",
                                    f"step_{'_'.join(map(str, step)) or 'all'}",
                                    f"rank_{'_'.join(map(str, rank)) or 'all'}",
                                    f"scope_{'_'.join(scope) or 'all'}",
                                    f"list_{'_'.join(lst) or 'all'}",
                                    f"data_mode_{'_'.join(data_mode)}"
                                ]
                                config_filename = "_".join(file_name_parts) + ".json"

                                with open(os.path.join(output_dir, config_filename), 'w') as f:
                                    json.dump(config, f, indent=4)

                                run_msprobe_ms_dump_files[3].append(config_filename.replace(".json", ".sh"))
                                config_id += 1

print(f"Generated {config_id} JSON configuration files in {output_dir}")

# 写入 .conf 文件
conf_filename = "run_msprobe_ms_dump.conf"
with open(conf_filename, 'w') as conf_file:

    conf_file.write("# run_msprobe_ms_dump.sh levels\n")
    conf_file.write(f'run_msprobe_ms_dump_level_1=""\n')
    conf_file.write(f'run_msprobe_ms_dump_level_2=""\n')
    conf_file.write(f'run_msprobe_ms_dump_level_3={" ".join(run_msprobe_ms_dump_files[3])}\n')
    conf_file.write(f'run_msprobe_ms_dump_level_4=""\n')

print(f"Generated {config_id} JSON configuration files in {output_dir}")
print(f"Configuration file {conf_filename} created.")
