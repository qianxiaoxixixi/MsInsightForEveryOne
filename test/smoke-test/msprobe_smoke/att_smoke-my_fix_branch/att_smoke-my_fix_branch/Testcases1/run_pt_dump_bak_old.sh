#!/bin/bash

# testcases_dir 为./att_smoke_new/Testcases
testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

# script_name 为run_pt_dump
script_name=$(basename "$0" .sh)

# 读取 JSON 配置, level_to_run表示需要运行的level级别, 由调用该脚本的地方传入
config_file="${testcases_dir}/pt_dump/utils/testcase_levels.json"
level_to_run=$1

# 判断传入的需要运行的level级别是否为空，为空则直接退出
if [[ -z "${level_to_run}" ]]; then
    echo "[Error] No level specified. Please provide a level (1, 2, 3, or 4)."
    exit 1
fi

# 获取 testcase_levels.json 中的测试用例名称
get_testcases() {
    local level=$1
    local script_name=$(basename "$0" .sh)
    jq -r --arg script_name "$script_name" --arg level "level_$level" '.[$script_name][$level][]' "${config_file}"
}

# 获取指定的层级在 testcase_levels.json 中的测试用例名称并用空格拼接起来
all_testcases=""
if [[ "$level_to_run" -le 2 ]]; then
    for level in $(seq 1 $level_to_run); do  
        testcases=$(get_testcases $level)
        if [[ -n "${testcases}" ]]; then
            all_testcases="${all_testcases} ${testcases}"
        fi
    done
elif [[ "$level_to_run" -le 4 ]]; then
    for level in $(seq 3 $level_to_run); do
        testcases=$(get_testcases $level)
        if [[ -n "${testcases}" ]]; then
            all_testcases="${all_testcases} ${testcases}"
        fi
    done
fi

# 检查 all_testcase 是否为空，为空则直接退出
if [[ -z "${all_testcases}" ]]; then
    echo "[Error] No test cases found for levels up to ${level_to_run}."
    exit 1
fi

# 找出 ./pt_dump/json/ 目录下的 JSON 文件和 ./pt_dump/utils/testcase_levels.json 的交集
json_files=$(ls ${testcases_dir}/pt_dump/json | grep '\.json$')
testcase_files=$(echo "${all_testcases}" | tr ' ' '\n' | grep '\.sh$')

# 去掉文件名后缀以进行比较
json_base_names=$(echo "${json_files}" | sed 's/\.[^.]*$//')
testcase_base_names=$(echo "${testcase_files}" | sed 's/\.[^.]*$//')

common_files=""
for json_base_name in ${json_base_names}; do
    for testcase_base_name in ${testcase_base_names}; do
        if [[ ${json_base_name} == ${testcase_base_name} ]]; then
            common_files="${common_files} ${json_base_name}.json"
        fi
    done
done

# output_dir 路径为../WorkSpace/output
if [ ! -d $output_dir ]; then
    mkdir -p $output_dir
fi

# result_log 路径在../WorkSpace/output/plog.txt
main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run Dump TestCase ----------------------------------"
    for casename in ${common_files}; do
        if [[ ${casename} =~ \.json$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"        
            bash ${testcases_dir}/pt_dump/test_dump.sh ${casename} ${level_to_run} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main
