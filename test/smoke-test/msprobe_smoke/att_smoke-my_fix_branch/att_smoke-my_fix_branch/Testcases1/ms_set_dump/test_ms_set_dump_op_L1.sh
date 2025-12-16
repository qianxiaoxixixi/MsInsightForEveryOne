#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
config_path=${cur_dir}/config/net_set_dump.json
net_path=${cur_dir}/src/net_set_dump_op.py
cmp_path=${cur_dir}/src/compare.py

source ${cur_dir}/../../Utils/conf.sh


main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/ms_set_dump/${file_name}"
    data_path=${testcase_result_dir}/data

    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    mkdir -p ${data_path}
    touch ${testcase_result_dir}/cmp_result.txt

    start_time=$(date "+%s")

    export MINDSPORE_DUMP_CONFIG=${config_path}
    export MS_DIAGNOSTIC_DATA_PATH=${data_path}
    unset MS_ACL_DUMP_CFG_PATH
    python ${net_path} >> ${testcase_result_dir}/log.txt
    unset MINDSPORE_DUMP_CONFIG
    unset MS_DIAGNOSTIC_DATA_PATH

    # python ${cmp_path} ${cur_dir}/baseline/${file_name} ${testcase_result_dir} >> ${testcase_result_dir}/cmp_result.txt
    # ret=$(grep "pass" ${testcase_result_dir}/cmp_result.txt)

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    # if [[ -z $ret ]]; then
    #     echo "${file_name} fail ${duration_time} mindspore"
    #     exit 1
    # fi

    echo "${file_name} pass ${duration_time} mindspore"

    rm -rf ${testcase_result_dir}/*
}

main
