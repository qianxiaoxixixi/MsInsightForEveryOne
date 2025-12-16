#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))

output_dir=$1
task=$2

check_compare_files() {
    local output_dir=$1
    local expected_count=$2
    local error_message=$3
    local file_name=$4

    local file_count=$(find "$output_dir" -type f | wc -l)

    if [ "$file_count" -ne "$expected_count" ]; then
        echo "${file_name} fail: [ERROR] $error_message"
        echo "${file_name} fail ${duration_time}s mindspore"
        return 1
    fi

    return 0
}

# compare check
compare_check() {
    local compare_output=$1
    local duration_time=$2

    if [[ "$compare_output" == *"compare_all"* ]]; then
        check_compare_files "${compare_output}" 3 "Compare all interrupted." "test_dump_compare_all"
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    if [[ "$compare_output" == *"compare_statistics_L0"* ]]; then
        check_compare_files "${compare_output}" 3 "Compare statistics_L0 interrupted." "test_dump_compare_statistics_L0"
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    if [[ "$compare_output" == *"compare_statistics_L1"* ]]; then
        check_compare_files "${compare_output}" 9 "Compare statistics_L1 interrupted." "test_dump_compare_statistics_L1"
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    return 0
}

main() {
    testcase_result_dir="${output_dir}/ms_dump_compare/${task}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time_compare=$(date '+%s')
    if [[ ${task} == "test_dump_compare_all" ]]; then
        python3 ${cur_dir}/src/run_${task}.py "${output_dir}/ms_dump/L1_task_tensor_step_all_rank_all_scope_all_list_all_data_mode_all" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
    elif [[ ${task} == "test_dump_compare_statistics_L0" ]]; then
        python3 ${cur_dir}/src/run_${task}.py "${output_dir}/ms_dump/L0_task_statistics_step_all_rank_all_scope_all_list_all_data_mode_all" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
    else
        python3 ${cur_dir}/src/run_${task}.py "${output_dir}/ms_dump/Larger_L1_task_statistics_step_all_rank_all_scope_all_list_all_data_mode_all" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt

    fi
    end_time_compare=$(date '+%s')
    duration_time=$(( ${end_time_compare} - ${start_time_compare} ))
    compare_check ${testcase_result_dir} ${duration_time}
    if [ $? -eq 0 ]; then
        echo "$task pass ${duration_time}s mindspore"
    fi
}


main
