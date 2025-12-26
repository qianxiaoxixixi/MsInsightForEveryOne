#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common_utils.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/api_accuracy_checking/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    rm -rf ${testcase_result_dir}/accuracy_checking_result_*.csv
    rm -rf ${testcase_result_dir}/accuracy_checking_details_*.csv
    start_time=$(date "+%s")
    api_info_part=${cur_dir}/json/${file_name}/dump_part.json
    api_info=${cur_dir}/json/${file_name}/dump.json

    msprobe -f pytorch run_ut -api_info ${api_info_part} -o ${testcase_result_dir} >> ${testcase_result_dir}/plog.txt
    echo "first run_ut finished" >> ${testcase_result_dir}/plog.txt
    msprobe -f pytorch run_ut -api_info ${api_info} -o ${testcase_result_dir} -csv_path ${testcase_result_dir}/accuracy_checking_result_*.csv >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    verify_ut_csv 12 70

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main