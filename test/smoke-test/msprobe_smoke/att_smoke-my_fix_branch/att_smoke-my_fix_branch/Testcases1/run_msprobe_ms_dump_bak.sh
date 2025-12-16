#!/bin/bash

# 获取当前脚本名称，不包含路径和后缀
script_name=$(basename "$0" .sh)
echo $script_name
testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

# 读取 JSON 配置
config_file="${testcases_dir}/msprobe_ms_dump/utils/testcase_levels.json"
level_to_run=$1

# 创建输出目录（如果不存在的话）
if [ ! -d "$output_dir" ]; then
    mkdir "$output_dir"
fi

# 获取脚本的第一个参数，作为要运行的层级
level_to_run=$1

if [[ -z "${level_to_run}" ]]; then
    echo "[Error] No level specified. Please provide a level (1, 2, 3, or 4)."
    exit 1
fi

# 获取配置中的测试用例
get_testcases() {
    local level=$1
    local script_name=$(basename "$0" .sh)
    echo "[Debug] Script name: ${script_name}"
    jq -r --arg script_name "$script_name" --arg level "level_$level" \
        '.[$script_name][$level][]' "${config_file}"
}

# 检查指定的层级是否在配置文件中
all_testcases=""
for level in $(seq 1 $level_to_run); do
    testcases=$(get_testcases $level)
    if [[ -n "${testcases}" ]]; then
        all_testcases="${all_testcases} ${testcases}"
    fi
done

if [[ -z "${all_testcases}" ]]; then
    echo "[Error] No test cases found for levels up to ${level_to_run}."
    exit 1
fi

# 找出 JSON 文件和层级文件名的交集
json_files=$(ls ${testcases_dir}/msprobe_ms_dump/json | grep '\.json$') #一级目录
# json_files=$(find ${testcases_dir}/msprobe_ms_dump/json -type f -name '*.json' | xargs -n 1 basename) #目录递归查找

testcase_files=$(echo "${all_testcases}" | tr ' ' '\n' | grep '\.sh$')

# 去掉文件名后缀以进行比较
json_base_names=$(echo "${json_files}" | sed 's/\.[^.]*$//')


testcase_base_names=$(echo "${testcase_files}" | sed 's/\.[^.]*$//')

# 打印调试信息
echo "[Debug] JSON base names:"
echo "${json_base_names}"

echo "[Debug] Testcase base names:"
echo "${testcase_base_names}"

common_files=""
for json_base_name in ${json_base_names}; do
    for testcase_base_name in ${testcase_base_names}; do
        if [[ ${json_base_name} == ${testcase_base_name} ]]; then
            # Append matching files with their original extensions
            common_files="${common_files} ${json_base_name}.json"
        fi
    done
done
echo "[Debug] Common base names:"
echo "${common_files}"


if [[ -z "${common_files}" ]]; then
    echo "[Warning] No common files found between JSON and testcase files."
else
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run msprobe_ms_dump TestCase ----------------------------"
    for casename in ${common_files}; do
        echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
             bash "${testcases_dir}/msprobe_ms_dump/test_dump.sh" "${casename}"
	#    bash "${testcases_dir}/msprobe_ms_dump/test_dump.sh" "${casename}" >> "${result_log}"
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
fi
