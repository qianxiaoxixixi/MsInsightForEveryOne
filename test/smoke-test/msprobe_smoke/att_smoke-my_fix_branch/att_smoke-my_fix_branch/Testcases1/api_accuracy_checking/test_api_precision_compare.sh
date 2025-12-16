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
    start_time=$(date "+%s")
    npu_csv_path=${cur_dir}/baseline/test_compare_benchmark_compare/accuracy_checking_details_20241220103133.csv
    gpu_csv_path=${cur_dir}/baseline/test_compare_benchmark_compare/accuracy_checking_details_20241220103133.csv

    msprobe -f pytorch api_precision_compare -npu ${npu_csv_path} -gpu ${gpu_csv_path} -o ${testcase_result_dir} >> ${testcase_result_dir}/plog.txt
    
    verify_compare_csv 26 34


    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
