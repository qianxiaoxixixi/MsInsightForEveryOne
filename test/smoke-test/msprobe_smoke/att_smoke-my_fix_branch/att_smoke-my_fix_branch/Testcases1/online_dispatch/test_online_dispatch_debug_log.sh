#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common.sh

# python3 ${cur_dir}/src/${test_name}.py ${testcase_result_dir} > ${testcase_result_dir}/plog.txt 2>&1
main() {

    sh_name="online_dispatch_llama2_test_debug_log"
    test_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/online_dispatch/${test_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time=$(date "+%s")
    cd ${cur_dir}/../../Model_online_dispatch/Model/ModelLink
    sh examples/llama2/${sh_name}.sh  > ${testcase_result_dir}/plog.txt 2>&1
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    compare_debug_log $testcase_result_dir/plog.txt

    echo "${test_name} pass ${duration_time} ${pytorch_branchs}"
}

main
