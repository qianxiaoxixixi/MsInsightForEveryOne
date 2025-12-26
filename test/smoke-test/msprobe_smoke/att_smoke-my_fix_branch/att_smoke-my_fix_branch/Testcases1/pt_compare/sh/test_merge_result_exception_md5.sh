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

    input_dir_path=${cur_dir}/test_data/test_merge_result_md5_with_stack
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

    grep "Rank0 compare result is 'md5' dump task" ${testcase_result_dir}/plog.txt && grep "Rank1 compare result is 'md5' dump task" ${testcase_result_dir}/plog.txt && grep "Nothing to merge" ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[ERROR] md5 dump task check error in log file." >> ${testcase_result_dir}/run_log.txt
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
