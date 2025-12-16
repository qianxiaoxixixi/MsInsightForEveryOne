#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))



source ${cur_dir}/../../Utils/conf.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/nan_analyze/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time=$(date "+%s")
    cd ${cur_dir}/test_data/
     msprobe -f pytorch nan_analyze -i ${cur_dir}/test_data/test_normal/step0 -o ${testcase_result_dir} >> ${testcase_result_dir}/plog.txt

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    grep "Cannot find any anomaly node, no need to generate analyze file." ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[ERROR]: tool running abnormally."
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main

