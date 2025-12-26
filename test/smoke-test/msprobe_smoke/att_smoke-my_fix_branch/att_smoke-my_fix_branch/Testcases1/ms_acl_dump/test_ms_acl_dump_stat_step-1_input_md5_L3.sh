#!/bin/bash

device_id=$1
if [ ! $device_id ]; then
    device_id=0
fi

cur_dir=$(dirname $(readlink -f $0))
config_path=${cur_dir}/config/test_ms_acl_dump_stat_step-1_input_md5.json
net_dir=${cur_dir}/src
net_path=${net_dir}/mnist_graph_step.py
cmp_path=${cur_dir}/src/compare.py


source ${cur_dir}/../../Utils/conf.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/ms_acl_dump/${file_name}"
    data_path=${testcase_result_dir}/dump_data

    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    mkdir -p ${data_path}
    touch ${testcase_result_dir}/cmp_result.txt

    sed -i 's|\("dump_path": "\).*|\1'"$data_path"'",|g' $config_path

    start_time=$(date "+%s")

    cd $net_dir
    python ${net_path} ${config_path} ${device_id} > ${testcase_result_dir}/worker.log 2>&1
    cd $cur_dir

    python ${cmp_path} ${cur_dir}/baseline/${file_name}/dump_data ${data_path} stat >> ${testcase_result_dir}/cmp_result.txt
    ret=$(grep "pass" ${testcase_result_dir}/cmp_result.txt)

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    if [[ -z $ret ]]; then
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} mindspore"

    # rm -rf ${testcase_result_dir}/*
}

main
