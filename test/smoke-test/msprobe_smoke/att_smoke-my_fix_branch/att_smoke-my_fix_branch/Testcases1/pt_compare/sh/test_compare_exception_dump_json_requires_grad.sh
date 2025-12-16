#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(dirname $(readlink -f $0)))

export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh


main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/pt_compare/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time=$(date "+%s")
    cd ${cur_dir}/src/

    npu_path=${cur_dir}/test_data/test_compare_exception/dump_json_requires_grad/dump.json
    bench_path=${cur_dir}/test_data/test_compare_exception/dump_json_requires_grad/dump.json
    stack_path=${cur_dir}/test_data/test_compare_exception/dump_json_requires_grad/stack.json

    input_path=${cur_dir}/json/compare_input_json/compare.json
    sed -i "s#\"npu_path\".*#\"npu_path\":\"${npu_path}\",#" $input_path
    sed -i "s#\"bench_path\".*#\"bench_path\":\"${bench_path}\",#" $input_path
    sed -i "s#\"stack_path\".*#\"stack_path\":\"${stack_path}\",#" $input_path
    out_dump_path=${testcase_result_dir}/out

    if [ -d ${out_dump_path} ]; then
        rm -rf ${out_dump_path}/*
    else
        mkdir -p ${out_dump_path}
    fi

    msprobe -f pytorch compare -i ${input_path} -o ${out_dump_path} >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    grep "not bool" ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[requires_grad object type] Check Error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
