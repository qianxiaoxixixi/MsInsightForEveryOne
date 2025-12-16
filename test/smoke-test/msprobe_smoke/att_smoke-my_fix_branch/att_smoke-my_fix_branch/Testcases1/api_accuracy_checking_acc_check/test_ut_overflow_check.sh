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
    api_info=${cur_dir}/json/${file_name}/dump.json
    # sed -i "s#\"dump_data_dir\".*#\"dump_data_dir\":\"${cur_dir}/json/${file_name}/dump_tensor_data\",#" $api_info

    echo "00000"
    msprobe -f pytorch run_overflow_check -api_info ${api_info} >> ${testcase_result_dir}/plog.txt
    echo "1111"

   if [ $? -ne 0 ];then
       echo "[ERROR] run_overflow_check.py failed during operation."
       echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
       exit 1
   fi

   grep "The Torch.div.0 overflow is a normal overflow." ${testcase_result_dir}/plog.txt
   if [ 0 -ne $? ]; then
       echo "Can not find 'The Torch.div.0 overflow is a normal overflow.'"
       echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
       exit 1
   fi

    grep "Distributed api is not supported for run ut. SKIP." ${testcase_result_dir}/plog.txt
   if [ 0 -ne $? ]; then
       echo "Can not find 'Distributed api is not supported for run ut. SKIP.'"
       echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
       exit 1
   fi

    # pkill -9  msprobe

    sta_api_info=${cur_dir}/json/${file_name}/dump_sta.json
    msprobe -f pytorch run_overflow_check -api_info ${sta_api_info} >> ${testcase_result_dir}/plog.txt


    grep "The Torch.arange.0 overflow is a normal overflow." ${testcase_result_dir}/plog.txt
    if [ 0 -ne $? ]; then
       echo "Can not find 'The Torch.arange.0 overflow is a normal overflow.'"
       echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
       exit 1
    fi

    echo "2222"
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main