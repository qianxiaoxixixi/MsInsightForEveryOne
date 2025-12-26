#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))

export HCCL_DETERMINISTIC=true
export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0
export ASCEND_LAUNCH_BLOCKING=1

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common.sh

# init_env_daily
# 需要执行的测试用例的 config.json 配置文件名
json_path=$1
# 需要运行的level级别
level_num=$2

main() {
    file_name=$(basename $json_path .json)
    # testcase_result_dir 为../../WorkSpace/output/pt_dump/
    testcase_result_dir="${output_dir}/pt_dump/${file_name}"
 
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    # 将 ./json/ 中的 json 配置文件中的 dump_path 换成WorkSpace/output/pt_dump/{file_name}/
    json_path=${cur_dir}/json/${file_name}.json
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${testcase_result_dir}\",#" $json_path

    start_time=$(date "+%s")
    if [[ "$file_name" == *"large"* ]]; then
        echo "Running llama2 large model with parameter: ${file_name}"
	    cp $json_path ${cur_dir}/../../Model/Modellink/dump_config.json
        current_path=$(pwd) 
        cd ${cur_dir}/../../Model/Modellink
        bash examples/llama2/pretrain_llama2_7b_ptd.sh >> ${testcase_result_dir}/plog.txt 2>&1
            cd "$current_path"
    elif [[ "$file_name" == *"overflow_check"* ]]; then  
        echo "Running overflow small model with parameter: ${file_name}"
        python3 ${cur_dir}/src/run_overflow_model.py ${json_path} >> ${testcase_result_dir}/plog.txt
    else
        echo "Running lenet small model with parameter: ${file_name}"
        python3 ${cur_dir}/src/run_lenet.py ${json_path} >> ${testcase_result_dir}/plog.txt
    fi
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    compare_dump ${testcase_result_dir} ${cur_dir}/baseline/level_${level_num}/${file_name}
    if [ $? -eq 0 ];then
        echo "${file_name} pass ${duration_time}s ${pytorch_branchs}"
    fi
}

main
