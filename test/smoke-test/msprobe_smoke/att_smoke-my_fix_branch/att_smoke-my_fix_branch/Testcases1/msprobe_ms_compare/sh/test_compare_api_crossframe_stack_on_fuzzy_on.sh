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

    npu_path=${cur_dir}/test_data/test_compare_api_crossframe_all_mode/npu/step0/rank/dump.json
    bench_path=${cur_dir}/test_data/test_compare_api_crossframe_all_mode/cpu/step0/rank/dump.json
    stack_path=${cur_dir}/test_data/test_compare_api_crossframe_all_mode/npu/step0/rank/stack.json
    user_mapping=${cur_dir}/user_mapping.yaml

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

    msprobe -f mindspore compare -i ${input_path} -o ${out_dump_path} -s -f >> ${testcase_result_dir}/plog.txt -am ${user_mapping}
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    input_result_path=${out_dump_path}/compare_result_*.xlsx
    python3 ${cur_dir}/src/compare_result_to_csv.py ${input_result_path} >> ${testcase_result_dir}/plog.txt
    rm ${input_result_path}

    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [ 0 -eq $? ]; then
        echo "[ERROR] Check Error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    count=$(grep "The summary of the expert advice is as follows" ${testcase_result_dir}/plog.txt | wc -l )
    if [ $count -le 0 ]; then
        echo "[ERROR] The summary of the expert advice is not generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    count=$(wc -l ${out_dump_path}/compare_result_*.csv | awk -F " " '{print $1}')
    result=$(ls $out_dump_path)
    if [[ $(ls ${out_dump_path} | wc -w) != 2 ]] || [[ ${result##*.} != "csv" ]]; then
        echo "[ERROR] The result file is not generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    grep "NPU_Stack_Info" ${testcase_result_dir}/out/compare_result_*.csv
    if [ $? -ne 0 ]; then
        echo "${file_name} fail ${duration_time} mindspore"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} mindspore"
}

main
