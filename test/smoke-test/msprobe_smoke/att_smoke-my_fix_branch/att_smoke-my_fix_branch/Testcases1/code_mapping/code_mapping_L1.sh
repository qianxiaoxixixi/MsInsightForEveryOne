#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
config_path=${cur_dir}/config/config.json
net_path=${cur_dir}/src/net_never_overflow.py
cmp_path=${cur_dir}/src/compare.py

source ${cur_dir}/../../Utils/conf.sh


main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/code_mapping/${file_name}"
    # data_path=${testcase_result_dir}/data

    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time=$(date "+%s")

    python ${net_path} >> ${testcase_result_dir}/log.txt

    python ${cmp_path} ${cur_dir}/baseline/${file_name} ${testcase_result_dir} >> ${testcase_result_dir}/cmp_result.txt
    ret=$(grep "pass" ${testcase_result_dir}/cmp_result.txt)

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    if [[ -z $ret ]]; then
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} mindspore"

    rm -rf ${testcase_result_dir}/*
}

main
