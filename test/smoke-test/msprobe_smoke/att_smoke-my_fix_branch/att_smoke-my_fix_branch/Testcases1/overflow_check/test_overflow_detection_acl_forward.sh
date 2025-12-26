#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/overflow_check/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    dump_config_path=${cur_dir}/json/dump.json
    dump_path=${testcase_result_dir}/acloutput
    mkdir -p ${dump_path}
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${dump_path}\",#" $dump_config_path
    start_time=$(date "+%s")
    cd ${cur_dir}/src/
    python3 ${cur_dir}/src/${file_name}.py ${cur_dir}/json/${file_name}.json >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    info=$(grep -o "overflow 1 times" ${testcase_result_dir}/plog.txt)
    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [[ 0 -eq $? ]] && [[ -z $info ]]; then
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    pkl_num=$(ls ${testcase_result_dir}/acloutput | wc -l)
    if [[ pkl_num -ne 1 ]]; then
        echo "[ERROR] acloutput directory is not generated."
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi
#
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
