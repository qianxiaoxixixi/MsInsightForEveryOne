#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(dirname $(readlink -f $0)))

export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh


main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/msprobe_ms_compare/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time=$(date "+%s")
    cd ${cur_dir}/src/

    npu_path=${cur_dir}/test_data/test_compare_summary_mode/npu/step0/rank/dump.json
    bench_path=${cur_dir}/test_data/test_compare_summary_mode/cpu/step0/rank/dump.json
    stack_path=${cur_dir}/test_data/test_compare_summary_mode/npu/step0/rank/stack.json

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

    msprobe -f mindspore compare -i ${input_path} -o ${out_dump_path} >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    input_result_path=${out_dump_path}/compare_result_*.xlsx
    python3 ${cur_dir}/src/compare_result_to_csv.py ${input_result_path} >> ${testcase_result_dir}/plog.txt

    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [ 0 -eq $? ]; then
        echo "[ERROR] Check Error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    count=$(wc -l ${out_dump_path}/compare_result_*.csv | awk -F " " '{print $1}')
    result=$(ls $out_dump_path)
    if [[ $(ls ${out_dump_path} | wc -w) != 3 ]]; then
        echo "[ERROR] The result file is not generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    md5_value=$(md5sum ${testcase_result_dir}/out/advisor_*.txt | awk '{ print $1 }')
    md5_baseline=$(md5sum ${cur_dir}/baseline/test_compare_summary/out/advisor_*.txt | awk '{ print $1 }')
    if [ "$md5_value" != "$md5_baseline" ]; then
        echo "The file txt MD5 values do not match"
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    md5_value=$(md5sum ${testcase_result_dir}/out/compare_result_*.csv | awk '{ print $1 }')
    md5_baseline=$(md5sum ${cur_dir}/baseline/test_compare_summary/out/compare_result_*.csv | awk '{ print $1 }')
    if [ "$md5_value" != "$md5_baseline" ]; then
        echo "The file csv MD5 values do not match"
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} mindspore"
}

main
