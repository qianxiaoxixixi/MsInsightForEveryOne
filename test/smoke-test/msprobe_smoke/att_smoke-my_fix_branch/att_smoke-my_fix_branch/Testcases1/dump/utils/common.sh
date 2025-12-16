#!/bin/bash

# Requires one parameters: error information
handle_error() {
    duration_time=${duration_time:=0}
    echo "${file_name} fail: $1"
    echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
    exit 1
}

compare_md5() {
    file1=$1
    file2=$2

    input_md5=$(md5sum $file1 | awk '{print $1}')
    baseline_md5=$(md5sum $file2 | awk '{print $1}')
    if [[ $input_md5 != $baseline_md5 ]]; then
        return 1
    fi
    return 0
}

compare_lines() {
    file1=$1
    file2=$2

    input_lines=$(cat $file1 | wc -l)
    baseline_lines=$(cat $file2 | wc -l)
    if [[ $input_lines != $baseline_lines ]]; then
        return 1
    fi
    return 0
}

compare_dump() {
    input_dir=$1
    baseline_dir=$2

    # check dump.json
    if [ ! -f ${input_dir}/step0/rank/dump.json ]; then
        handle_error "[ERROR] dump json is not exist."
    else
        compare_md5 ${input_dir}/step0/rank/dump.json ${baseline_dir}/step0/rank/dump.json
        if [ $? -eq 1 ];then
            handle_error "${casename} dump.json check fail"
        fi
    fi

    # check dump_tensor_data
    if [ -d ${baseline_dir}/step0/rank/dump_tensor_data ]; then
        if [ ! -d ${input_dir}/step0/rank/dump_tensor_data ]; then
            handle_error "[ERROR] dump_tensor_data dir is not exist."
        else
            dump_tensor_data_count=$(ls ${input_dir}/step0/rank/dump_tensor_data | wc -l)
            dump_tensor_data_baseline_count=$(ls ${baseline_dir}/step0/rank/dump_tensor_data | wc -l)
            if [ $dump_tensor_data_count != $dump_tensor_data_baseline_count ];then
                handle_error "${casename} dump_tensor_data check fail, baseline num: ${dump_tensor_data_baseline_count}, new output nums: ${dump_tensor_data_count}"
            fi
        fi
    fi

    # check construct.json
    if [ ! -f ${input_dir}/step0/rank/construct.json ]; then
        handle_error "[ERROR] construct.json is not exist."
    else
        compare_md5 ${input_dir}/step0/rank/construct.json ${baseline_dir}/step0/rank/construct.json
        if [ $? -eq 1 ];then
            handle_error "${casename} construct.json check fail"
        fi
    fi

    # check stack.json
    if [ ! -f ${input_dir}/step0/rank/stack.json ]; then
        handle_error "[ERROR] stack.json is not exist."
    else
        compare_lines ${input_dir}/${casename}/step0/rank/stack.json ${baseline_dir}/${casename}/step0/rank/stack.json
        if [ $? -eq 1 ];then
            handle_error "${casename} stack.json check fail"
        fi
    fi


}

