#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))

output_dir=$1
task=$2
pytorch_branchs=$3

check_compare_files() {
    local output_dir=$1
    local expected_count=$2
    local error_message=$3
    local file_name=$4
    local pytorch_branchs=$5

    local file_count=$(find "$output_dir" -type f | wc -l)

    if [ "$file_count" -ne "$expected_count" ]; then
        echo "${file_name} fail: [ERROR] $error_message"
        echo "${file_name} fail ${duration_time}s ${pytorch_branchs}"
        return 1
    fi

    return 0
}

# compare check
compare_check() {
    local compare_output=$1
    local duration_time=$2
    local pytorch_branchs=$3

    if [[ "$compare_output" == *"compare_all"* ]]; then
        check_compare_files "${compare_output}" 3 "Compare all interrupted." "test_dump_compare_all" $pytorch_branchs
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    if [[ "$compare_output" == *"compare_md5"* ]]; then
        check_compare_files "${compare_output}" 9 "Compare md5 interrupted." "test_dump_compare_md5" $pytorch_branchs
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    if [[ "$compare_output" == *"compare_statistics"* ]]; then
        check_compare_files "${compare_output}" 3 "Compare statistics interrupted." "test_dump_compare_statistics" $pytorch_branchs
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    return 0
}

main() {
    testcase_result_dir="${output_dir}/pt_dump_compare/${task}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time_compare=$(date '+%s')
    if [[ ${task} == "test_dump_compare_all" ]]; then
        python3 ${cur_dir}/src/run_${task}.py "${output_dir}/pt_dump/test_pytorch_mix_tensor_none" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
    elif [[ ${task} == "test_dump_compare_md5" ]]; then
        python3 ${cur_dir}/src/run_${task}.py "${output_dir}/pt_dump/test_large_pytorch_mix_statistics_none" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
    else
        python3 ${cur_dir}/src/run_${task}.py "${output_dir}/pt_dump/test_large_pytorch_L0_statistics_rank_none" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
    fi
    end_time_compare=$(date '+%s')
    duration_time=$(( ${end_time_compare} - ${start_time_compare} ))
    compare_check ${testcase_result_dir} ${duration_time} ${pytorch_branchs}
    if [ $? -eq 0 ]; then
        echo "$task pass ${duration_time}s ${pytorch_branchs}"
    fi
}


main
