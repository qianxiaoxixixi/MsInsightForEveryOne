#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common_utils.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/msmonitor/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    export MONITOR_OUTPUT_DIR=${testcase_result_dir}
    start_time=$(date "+%s") 

    cd $MODEL_PATH
    bash examples/llama2/test_print_struct.sh  > ${testcase_result_dir}/plog.txt 2>&1


    # if no stop exception, failed
    exception_nums=$(cat ${testcase_result_dir}/plog.txt | grep "Exception: exit after first monitor step when print model struct" | wc -l)
    # exception_nums=$(cat ${testcase_result_dir}/plog.txt | grep "Exception: exit after first step when print model struct" | wc -l)
    if  [[ exception_nums -ne 1 ]]; then
        handle_error "[ERROR] the number of stopping exception is incorrect."
    fi


    # if no dir module_struct failed
    if [ ! -d ${testcase_result_dir}/module_struct ]; then
        handle_error "[ERROR] module_struct is not exist."
    fi

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
