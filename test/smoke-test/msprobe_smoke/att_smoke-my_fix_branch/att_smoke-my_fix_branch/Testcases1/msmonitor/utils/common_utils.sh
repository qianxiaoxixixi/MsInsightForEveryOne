#!/bin/bash
MODEL_PATH=/home/maoyanlong/att_smoke_new/Model_msmonitor/Model/ModelLink

handle_error() {
    echo "$1"
    duration_time=${duration_time:=0}
    echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
    echo "${file_name} fail: $1"
    exit 1
}

verify_event_output() {
    echo ${testcase_result_dir}
    ls -l ${testcase_result_dir}
    ls -l ${testcase_result_dir} |grep event 
    event_output_num=$(ls -l ${testcase_result_dir}/*/|grep event | wc -l)
    if  [[ event_output_num -ne $1 ]]; then
        handle_error "[ERROR] the number ${event_output_num} is incorrect."
    fi
}

verify_csv_output() {
    echo ${testcase_result_dir}
    ls -l ${testcase_result_dir}
    ls -l ${testcase_result_dir} |grep csv 
    csv_output_num=$(ls -l ${testcase_result_dir}/*/|grep csv | wc -l)
    if  [[ csv_output_num -ne $1 ]]; then
        handle_error "[ERROR] the number ${csv_output_num} is incorrect."
    fi
}

verify_csv_content() {
    local check_file=$1
    local expect_num=$2
    local expected_header=$3
    if [ ! -f ${check_file} ]; then
        handle_error "[ERROR] ${check_file} is not exist."
    fi
    actual_head=$(head -n 1 ${check_file} | tr -d '\r')

    if [ "$expected_header" != "$actual_head" ]; then
        handle_error "[ERROR] ${check_file} header abnormality"
    fi
    actual_num=$(cat ${check_file} |wc -l)
    if  [[ $actual_num -ne $expect_num ]]; then
        handle_error "[ERROR] the number ${actual_num} of line in ${check_file} is incorrect."
    fi

}

time_performance_check() {
    local base_use_time=$1
    local tool_use_time=$2
    local threshold=0.1

    expansion_rate=$(echo "scale=3; ($tool_use_time-$base_use_time) / $base_use_time" | bc)
    echo $expansion_rate
    # if  [[ ${expansion_rate} > ${threshold} ]]; then
    if (( $(echo "$expansion_rate > $threshold" | bc -l) )); then
        handle_error "[ERROR] the expansion_rate ${expansion_rate} exceeds the threshold ${threshold}."
    fi
}
