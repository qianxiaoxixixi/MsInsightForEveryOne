#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
config_path=${cur_dir}/config/config.json
cmp_path=${cur_dir}/src/compare.py

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/../../Utils/common.sh
source ${cur_dir}/utils/common.sh

main() {
    start_time=$(date "+%s")

    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/msa_overflow_check/${file_name}"
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

    cp -f $config_path /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM/msa_dump_2/config.json
    current_path=$(pwd) 
    cd /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM
    sed -i "s/enable_overflow = .*/enable_overflow = True/" mindspeed_llm/training/training.py
    sed -i "s|PrecisionDebugger(config_path=.*|PrecisionDebugger(config_path=\'./msa_dump_2/config.json\')|" mindspeed_llm/training/training.py
    bash examples/mindspore/qwen25/pretrain_qwen25_7b_32k_ms.sh
    sed -i "s/enable_overflow = .*/enable_overflow = False/" mindspeed_llm/training/training.py
    cp -r /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM/msrun_log ${testcase_result_dir}/
    cd "$current_path"

    python ${cmp_path} ${cur_dir}/baseline/${file_name} ${testcase_result_dir} >> ${testcase_result_dir}/cmp_result.txt
    ret=$(grep "pass" ${testcase_result_dir}/cmp_result.txt)
    validate_output_directory_first  ${config_path} ${testcase_result_dir} ${cur_dir}/baseline/${file_name}


    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    if [[ -z $ret ]]; then
        echo "${file_name} fail ${duration_time} msadapter"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} msadapter"
}

main
