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

    npu_path=${cur_dir}/test_data/test_compare_summary_mode/npu/step0
    bench_path=${cur_dir}/test_data/test_compare_summary_mode/cpu/step0

    input_path=${cur_dir}/json/compare_input_json/compare_distributed.json
    sed -i "s#\"npu_path\".*#\"npu_path\":\"${npu_path}\",#" $input_path
    sed -i "s#\"bench_path\".*#\"bench_path\":\"${bench_path}\",#" $input_path
    out_compare_path=${testcase_result_dir}/out

    if [ -d ${out_compare_path} ]; then
        rm -rf ${out_compare_path}/*
    else
        mkdir -p ${out_compare_path}
    fi

    msprobe -f pytorch compare -i ${input_path} -o ${out_compare_path} >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [ 0 -eq $? ]; then
        echo "[ERROR] Check Error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    result=$(ls $out_compare_path)
    if [[ $(ls ${out_compare_path} | wc -w) !=  2 ]] || [[ ${result##*.} != "xlsx" ]]; then
        echo "[ERROR] The result file is not generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    cd ${out_compare_path}
    xlsx_name=$(ls *.xlsx)
    if [[ $xlsx_name != *"_rank_"* ]]; then
        echo "[ERROR] The wrong file is generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
