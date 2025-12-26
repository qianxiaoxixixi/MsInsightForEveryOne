#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))

export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common.sh

# need input the config.json
json_path=$1

main() {

    file_name=$(basename $json_path .json)
    # testcase_result_dir="${output_dir}/free_benchmark/${file_name}"
    testcase_result_dir="/home/maoyanlong/att_smoke_new/jiankunlin/free_benchmarks/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    json_path=${cur_dir}/json/${file_name}.json
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${testcase_result_dir}\"#" $json_path
    start_time=$(date "+%s")
    if [[ ${json_path} == *"larger"* ]]; then
    #运行大模型
    	echo "Running large1 model with parameter: ${json_path}"
        cp $json_path ${cur_dir}/../../Model/Modellink/dump_config.json
        current_path=$(pwd) 
        cd ${cur_dir}/../../Model/Modellink
        bash examples/llama2/pretrain_llama2_7b_ptd.sh >> ${testcase_result_dir}/plog.txt 2>&1
        cd "$current_path"
        compare_free_benchmark_log ${testcase_result_dir}/plog.txt  ${cur_dir}/baseline/${file_name}/plog.txt
        compare_free_benchmark_result ${cur_dir}/baseline/${file_name} ${testcase_result_dir} 1
    elif [[ ${json_path} == *"exception"* ]]; then
        # 处理异常情况
        echo "Handling exception case with parameter: ${json_path}"

    else
    #运行小模型
        echo "Running small model with parameter: ${json_path}"

        python3 ${cur_dir}/src/run_sample.py ${json_path} >> ${testcase_result_dir}/plog.txt
        compare_free_benchmark_log ${testcase_result_dir}/plog.txt  ${cur_dir}/baseline/${file_name}/plog.txt
        compare_free_benchmark_result ${cur_dir}/baseline/${file_name} ${testcase_result_dir} 0
    fi
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
