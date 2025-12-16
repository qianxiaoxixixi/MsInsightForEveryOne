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
    # testcase_result_dir="${output_dir}/dump/${file_name}"
    testcase_result_dir="${cur_dir}/baseline/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    json_path=${cur_dir}/json/${file_name}.json
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${testcase_result_dir}\"#" $json_path

    start_time=$(date "+%s")
    python3 ${cur_dir}/src/run_sample.py ${json_path} >> ${testcase_result_dir}/plog.txt
#    if [ $? -ne 0 ];then
#        handle_error "[ERROR] ${file_name} run_sample.py failed during operation."
#    fi
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    compare_dump ${cur_dir}/baseline/${file_name} ${testcase_result_dir}

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
