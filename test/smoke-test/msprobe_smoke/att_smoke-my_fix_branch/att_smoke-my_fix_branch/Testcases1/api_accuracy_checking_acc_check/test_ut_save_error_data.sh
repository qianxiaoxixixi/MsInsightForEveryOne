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
    rm -rf ${att_ut_path}/ut_error_data*

    start_time=$(date "+%s")
    api_info=${cur_dir}/json/${file_name}.json
    msprobe -f pytorch acc_check -api_info ${api_info} -o ${testcase_result_dir} -j >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    verify_ut_csv 20 20

#    if [ ! -d ${att_ut_path}/ut_error_data* ]; then
#        handle_error "[ERROR] ut_error_data is not exist."
#    fi
#
#    if [ $(ls ${att_ut_path}/ut_error_data*/*.pt 2>/dev/null | wc -l) -eq 0 ]; then
#        handle_error "[ERROR] There are no .pt files in the folder ut_error_data*."
#    fi
#
#    if [ $(ls -1 ${att_ut_path}/ut_error_data* | grep -v ".pt$" | wc -l) -gt 0 ]; then
#        handle_error "[ERROR] The folder ut_error_data* contains other files or folders."
#    fi

    rm -rf ${att_ut_path}/ut_error_data*

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main