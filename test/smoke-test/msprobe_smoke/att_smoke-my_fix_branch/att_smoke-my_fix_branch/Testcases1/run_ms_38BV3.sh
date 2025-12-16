#!/bin/bash

start_time=$(date "+%F %T")
echo "[Info][${start_time}] Script started."

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh
source /home/maoyanlong/.bashrc
conda init
conda activate my_py39
cd /home/maoyanlong/att_smoke_new
ORI_PYTHON=$(echo "$PYTHONPATH")
export PYTHONPATH=/home/maoyanlong/att_smoke_new/WorkSpace1/pkg/debug/accuracy_tools:$PYTHONPATH
# bash compile_msprobe.sh $2 $3
# if [ -f /home/maoyanlong/att_smoke_new/mstt_for_compile/debug/accuracy_tools/dist/*.whl ]; then
    # pip3 uninstall mindstudio-probe -y
    # cd /home/maoyanlong/att_smoke_new/mstt_for_compile/debug/accuracy_tools/dist
    # pip3 install *.whl
# fi

# cd /home/maoyanlong/att_smoke_new 


script_name=$(basename "$0" .sh)

# 读取 JSON 配置, level_to_run表示需要运行的level级别, 由调用该脚本的地方传入
config_file="${testcases_dir}/ms_38BV3/utils/testcase_levels.json"
level_to_run=$1

# 判断传入的需要运行的level级别是否为空，为空则直接退出
if [[ -z "${level_to_run}" ]]; then
    # echo "[Error] No level specified. Please provide a level (1, 2, 3, or 4)."
    # exit 1
    level_to_run=2
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
else
    testcases=$(get_testcases $level_to_run)
    if [[ -n "${testcases}" ]]; then
        all_testcases="${all_testcases} ${testcases}"
    fi
fi

# 检查 all_testcase 是否为空，为空则直接退出
if [[ -z "${all_testcases}" ]]; then
    echo "[Error] No test cases found for levels up to ${level_to_run}."
    exit 1
fi

# 找出 ./ms_38BV3/json/ 目录下的 JSON 文件和 ./ms_38BV3/utils/testcase_levels.json 的交集
json_files=$(ls ${testcases_dir}/ms_38BV3/json | grep '\.json$')
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


# 初始化数组
testcases_arr=()
for file in ${common_files}; do
    testcases_arr+=("$file")
done
echo "The testcases to be executed are as follows: "
for casename in ${testcases_arr[@]}; do
    echo ${casename}
done

check_compare_files() {
    local output_dir=$1
    local expected_count=$2
    local error_message=$3
    local file_name=$4

    local file_count=$(find "$output_dir" -type f | wc -l)

    if [ "$file_count" -ne "$expected_count" ]; then
        echo "${file_name} fail: [ERROR] $error_message"
        echo "${file_name} fail ${duration_time}s ${pytorch_branchs}"
    fi
}

# compare check
compare_check() {
    compare_output=$1
    duration_time=$2
    pytorch_branchs=$3

    if [[ "$compare_output" == *"compare_all"* ]]; then
        check_compare_files "${compare_output}" 2 "Compare all interrupted." "compare_all"
        return 1
    fi

    if [[ "$compare_output" == *"compare_md5"* ]]; then
        check_compare_files "${compare_output}" 8 "Compare md5 interrupted." "compare_md5"
        return 1
    fi

    if [[ "$compare_output" == *"compare_statistics"* ]]; then
        check_compare_files "${compare_output}" 2 "Compare statistics interrupted." "compare_statistics"
        return 1
    fi

    return 0
}


# result_log 路径在../WorkSpace/output/plog.txt
main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run MS Pangu38BV3 TestCase ----------------------------------"

    for casename in ${testcases_arr[@]}; do
        # 输出当前文件名
        echo "[Debug][$(date "+%F %T")] Processing file: ${casename}"
        if [[ ${casename} =~ \.json$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/ms_38BV3/test_38BV3.sh ${casename} >> ${result_log}
        fi
    done

    # if [[ "$level_to_run" -eq 4 ]]; then
    # 	echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run Compare TestCase ----------------------------------"
    # 	tasks=("test_dump_compare_all" "test_dump_compare_md5" "test_dump_compare_statistics")
    # 	for task in ${tasks[@]}; do
    #     	echo "[Info][$(date '+%F %T')] ---------------------------- Execution ${task} ----------------------------"
    #     	bash ${testcases_dir}/ms_38BV3/test_dump_compare.sh ${output_dir} ${task} ${pytorch_branchs} >> ${result_log}
    # 	done
    # fi

    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main

conda deactivate
conda activate $conda_env_name
export PYTHONPATH=$ORI_PYTHON

end_time=$(date "+%F %T")
echo "[Info][${end_time}] Script completed."
