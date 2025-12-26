#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
config_path=${cur_dir}/config/config.json
net_path=${cur_dir}/src/net_free_benchmark.py
cmp_path=${cur_dir}/src/compare.py

source ${cur_dir}/../../Utils/conf.sh


main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/ms_free_benchmark/${file_name}"
    data_path=${testcase_result_dir}/data

    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    mkdir -p ${data_path}
    touch ${testcase_result_dir}/cmp_result.txt

    sed -i 's|\("dump_path": "\).*|\1'"$data_path"'",|g' $config_path
    sed -i 's|\("step": \).*|\1[],|g' $config_path
    sed -i 's|\("list": \).*|\1[],|g' $config_path
    sed -i 's|\("level": "\).*|\1L1",|g' $config_path
    sed -i 's|\("fuzz_device": "\).*|\1cpu",|g' $config_path
    sed -i 's|\("pert_mode": "\).*|\1improve_precision",|g' $config_path
    sed -i 's|\("handler_type": "\).*|\1check",|g' $config_path
    sed -i 's|\("fuzz_level": "\).*|\1L1",|g' $config_path
    sed -i 's|\("fuzz_stage": "\).*|\1forward"|g' $config_path

    start_time=$(date "+%s")

    python ${net_path} >& ${testcase_result_dir}/log.txt

    python ${cmp_path} ${cur_dir}/baseline/${file_name} ${testcase_result_dir} >> ${testcase_result_dir}/cmp_result.txt
    ret=$(grep "pass" ${testcase_result_dir}/cmp_result.txt)

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    if [[ -z $ret ]]; then
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    info=$(grep "Exception:\ fuzz_device\ must\ be\ npu" ${testcase_result_dir}/log.txt)
    if [[ -z $info ]]; then
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} mindspore"

    rm -rf ${testcase_result_dir}/*
}

main

