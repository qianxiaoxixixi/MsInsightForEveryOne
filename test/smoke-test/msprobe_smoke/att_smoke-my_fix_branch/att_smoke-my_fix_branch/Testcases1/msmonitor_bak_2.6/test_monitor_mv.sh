#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common_utils.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/msmonitor/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi
    export MONITOR_OUTPUT_DIR=${testcase_result_dir}
    echo $MONITOR_OUTPUT_DIR
    start_time=$(date "+%s") 

    cd $MODEL_PATH
    # cd /home/maoyanlong/att_smoke_new/Model_msmonitor/Model/ModelLink
    bash examples/llama2/test_mv.sh  > ${testcase_result_dir}/plog.txt 2>&1

    verify_csv_output 2

    # testcase_result_dir=/home/maoyanlong/att_smoke_new/WorkSpace1/output-16/msmonitor/test_mv
    expected_header="vpp_stage,param_name,step,norm,min,max,mean,nans"
    # expected_header="vpp_stage,param_name,step,exp_avg.norm,exp_avg.min,exp_avg.max,exp_avg.mean,exp_avg.nans"
    verify_csv_content ${testcase_result_dir}/*/exp_avg_1-1.csv 28 $expected_header
    expected_header="vpp_stage,param_name,step,norm,min,max,mean,nans"
    # expected_header="vpp_stage,param_name,step,exp_avg_sq.norm,exp_avg_sq.min,exp_avg_sq.max,exp_avg_sq.mean,exp_avg_sq.nans"
    verify_csv_content ${testcase_result_dir}/*/exp_avg_sq_1-1.csv 28 $expected_header


    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
