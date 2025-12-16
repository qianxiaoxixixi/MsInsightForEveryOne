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

    npu_dump_dir=${cur_dir}/test_data/test_compare_summary_first_diff_analyze/npu/step0
    bench_dump_dir=${cur_dir}/test_data/test_compare_summary_first_diff_analyze/cpu/step0
    out_compare_path=${testcase_result_dir}/out

    if [ -d ${out_compare_path} ]; then
        rm -rf ${out_compare_path}/*
    else
        mkdir -p ${out_compare_path}
    fi

    python3 ${cur_dir}/src/first_diff_analyze/first_diff_analyze.py ${npu_dump_dir} ${bench_dump_dir} ${out_compare_path} >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [ 0 -eq $? ]; then
        echo "[ERROR] Check Error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    count=$(wc -l ${out_compare_path}/compare_result_*.json | awk -F " " '{print $1}')
    result=$(ls $out_compare_path)
    if [[ $(ls ${out_compare_path} | wc -w) != 2 ]]; then
        echo "[ERROR] The json file is not generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    md5_value=$(md5sum ${testcase_result_dir}/out/compare_result_rank0_*.json | awk '{ print $1 }')
    md5_baseline=$(md5sum ${cur_dir}/baseline/test_compare_summary_first_diff_analyze/out/compare_result_rank0_*.txt | awk '{ print $1 }')
    if [ "$md5_value" != "$md5_baseline" ]; then
        echo "The rank0 file json MD5 values do not match"
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    md5_value=$(md5sum ${testcase_result_dir}/out/compare_result_rank1_*.json | awk '{ print $1 }')
    md5_baseline=$(md5sum ${cur_dir}/baseline/test_compare_summary_first_diff_analyze/out/compare_result_rank1_*.txt | awk '{ print $1 }')
    if [ "$md5_value" != "$md5_baseline" ]; then
        echo "The rank1 file json MD5 values do not match"
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
