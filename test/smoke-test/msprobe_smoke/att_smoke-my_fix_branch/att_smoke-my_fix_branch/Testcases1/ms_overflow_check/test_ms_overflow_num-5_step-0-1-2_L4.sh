#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
config_path=${cur_dir}/config/config.json
net_dir=${cur_dir}/src/large_pangu
net_path=${net_dir}/run_pp_overflow.sh
cmp_path=${cur_dir}/src/compare.py
model_path=${cur_dir}/../ms_model/dynamic-parallel

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/../../Utils/common.sh
source ${cur_dir}/utils/common.sh

# init_env_daily

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/ms_overflow_check/${file_name}"
    data_path=${testcase_result_dir}/data

    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    mkdir -p ${data_path}
    touch ${testcase_result_dir}/cmp_result.txt

    sed -i 's|\("dump_path": "\).*|\1'"$data_path"'",|g' $config_path
    sed -i 's|\("step": \).*|\1[0, 1, 2],|g' $config_path
    sed -i 's|\("overflow_nums": \).*|\15|g' $config_path
    sed -i 's|\(log_path=\).*|\1'${testcase_result_dir}/pp_log'|g' $net_path

    start_time=$(date "+%s")

    cd $net_dir
    ori_python_path=$(echo ${PYTHONPATH})
    export PYTHONPATH=${model_path}:${PYTHONPATH}
    bash ${net_path}
    export PYTHONPATH=$ori_python_path
    cd $cur_dir

    python ${cmp_path} ${cur_dir}/baseline/${file_name} ${testcase_result_dir} >> ${testcase_result_dir}/cmp_result.txt
    ret=$(grep "pass" ${testcase_result_dir}/cmp_result.txt)
    validate_output_directory_first  ${config_path} ${testcase_result_dir} ${cur_dir}/baseline/${file_name}


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
