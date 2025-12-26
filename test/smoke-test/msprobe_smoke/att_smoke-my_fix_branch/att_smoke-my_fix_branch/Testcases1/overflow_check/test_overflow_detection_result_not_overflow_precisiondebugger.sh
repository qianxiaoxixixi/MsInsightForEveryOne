#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh


main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/overflow_check/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    start_time=$(date "+%s")
    cd ${cur_dir}/src/
    python3 ${cur_dir}/src/${file_name}.py ${cur_dir}/json/${file_name}.json >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))


    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [ 0 -eq $? ]; then
        grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    grep 'The overflow_check hook function is successfully mounted to the model'  ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[ERROR] The keyword \"hook function successfully mounted to the model\" is not found in the command output."
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
