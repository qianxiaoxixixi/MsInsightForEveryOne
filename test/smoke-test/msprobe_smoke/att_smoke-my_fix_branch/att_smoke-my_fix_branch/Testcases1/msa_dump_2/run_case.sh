#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))

export HCCL_DETERMINISTIC=true
export ASCEND_LAUNCH_BLOCKING=1

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common.sh

# 需要执行的测试用例的 config.json 配置文件名
json_path=$1

main() {
    start_time=$(date "+%s")

    file_name=$(basename $json_path .json)
    testcase_result_dir="${output_dir}/msa_dump_2/${file_name}"
 
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    # 将 ./json/ 中的 json 配置文件中的 dump_path 换成WorkSpace/output/ms_38BV3/{file_name}/
    json_path=${cur_dir}/json/${file_name}.json
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${testcase_result_dir}\",#" $json_path

    cp -f $json_path /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM/msa_dump_2/config.json
    current_path=$(pwd) 
    cd /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM
    sed -i "s/enable_msprobe = .*/enable_msprobe = True/" mindspeed_llm/training/training.py
    sed -i "s|PrecisionDebugger(config_path=.*|PrecisionDebugger(config_path=\'./msa_dump_2/config.json\')|" mindspeed_llm/training/training.py
    bash examples/mindspore/qwen25/pretrain_qwen25_7b_32k_ms.sh
    cp -r /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM/msrun_log ${testcase_result_dir}/
    cd "$current_path"

    compare_dump ${testcase_result_dir} ${cur_dir}/baseline/${file_name}

    if [ $? -eq 0 ];then
        end_time=$(date "+%s")
        duration_time=$(( ${end_time} - ${start_time} ))
        echo "${file_name} pass ${duration_time}s msadapter"
    fi
}

main
