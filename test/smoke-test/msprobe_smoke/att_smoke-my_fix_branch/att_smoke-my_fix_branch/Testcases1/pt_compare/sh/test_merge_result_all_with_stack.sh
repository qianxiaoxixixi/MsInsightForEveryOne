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

    input_dir_path=${cur_dir}/test_data/test_merge_result_all_with_stack
    output_dir_path=${testcase_result_dir}/out
    config_yaml_path=${cur_dir}/yaml/config_all_indexes.yaml

    if [ -d ${output_dir_path} ]; then
        rm -rf ${output_dir_path}/*
    else
        mkdir -p ${output_dir_path}
    fi

    msprobe -f pytorch merge_result -i ${input_dir_path} -o ${output_dir_path} -config ${config_yaml_path}>> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    input_result_path=${output_dir_path}/multi_ranks_compare_merge_*.xlsx
    python3 ${cur_dir}/src/compare_result_to_csv_update.py ${input_result_path} >> ${testcase_result_dir}/plog.txt

    grep "\[ERROR\]" ${testcase_result_dir}/plog.txt
    if [ 0 -eq $? ]; then
        echo "[ERROR] Check Error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    count=$(wc -l ${output_dir_path}/multi_ranks_compare_merge_*.csv | awk -F " " '{print $1}')
    result=$(ls $output_dir_path)
    if [[ $(ls ${output_dir_path} | wc -w) != 2 ]]; then
        echo "[ERROR] The result file is not generated." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    baseline_xlsx_path=${cur_dir}/baseline/test_merge_result_all_with_stack/out/multi_ranks_compare_merge_*.xlsx
    python3 ${cur_dir}/src/xlsx_compare.py ${input_result_path} ${baseline_xlsx_path}>> ${testcase_result_dir}/plog.txt
    grep "The files are identical" ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[ERROR] xlsx files are nor identical." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
