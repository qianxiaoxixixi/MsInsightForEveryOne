#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))
source ${cur_dir}/../../Utils/conf.sh

src_dir=${cur_dir}/src

export ASCEND_SLOG_PRINT_TO_STDOUT=0
export ASCEND_GLOBAL_LOG_LEVEL=0

export PYTHONPATH=${download_path}/debug/accuracy_tools:$PYTHONPATH

output_dir=$1
file_name=$(basename $0 .sh)
testcase_result_dir="${output_dir}/${file_name}"
if [ ! -d ${testcase_result_dir} ]; then
    mkdir -p ${testcase_result_dir}
else
    rm -rf ${testcase_result_dir}/*
fi

config_filepath=${testcase_result_dir}/dump.json
config_content="{
\n \"task\": \"grad_probe\",
\n \"dump_path\": \"${testcase_result_dir}\", 
\n \"rank\": [],  
\n \"step\": [],  
\n \"grad_probe\": {
\n     \"grad_level\": \"L1\",
\n     \"param_list\": [],
\n     \"bounds\": [-1, 0, 1]
\n }
\n}"
echo -e $config_content > $config_filepath

pytorch_branchs=$2

n_procs="1"

header="param_name,max,min,norm,shape"

source ${cur_dir}/utils/common_utils.sh

step_num=4
weight_grad_num=4

main() {
    start_time=$(date "+%s")

    cmd="python ${src_dir}/test_L1_dump.py ${config_filepath} ${n_procs} >> ${testcase_result_dir}/plog.txt"
    echo $cmd

    eval $cmd  2>&1 > ${testcase_result_dir}/plog.txt
    #if [ $? -ne 0 ];then
    #    handle_error "dump failed."
    #fi

    rank_dirs=(`find ${testcase_result_dir} -type d -name 'rank*'`)
    rank_dirs_len=${#rank_dirs[@]}
    if [ ${rank_dirs_len} -ne ${n_procs} ]; then
        handle_error "rank dir number wrong"
    fi

    grad_summary_files=(`find ${rank_dirs[0]} -type f -name 'grad_summary*'`)
    grad_summary_files_len=${#grad_summary_files[@]}
    if [ ${grad_summary_files_len} -ne ${step_num} ]; then
        handle_error "grad_summary files number wrong"
    fi

    grad_summary_file=${grad_summary_files[0]}
    result_header=$(head -n 1 ${grad_summary_file})
    if [[ ! ${result_header} =~ ${header} ]]; then
        echo ${result_header}
        handle_error "grad_summary file header wrong"
    fi
    result_lines=(`wc -l ${grad_summary_file}`)
    result_lines_number=${result_lines[0]}
    if [ ${result_lines_number} -ne `expr ${weight_grad_num} + 1` ]; then
        handle_error "grad_summary file content wrong"
    fi

    step_dirs=(`find ${rank_dirs[0]} -type d -name 'step*'`)
    step_dirs_num=${#step_dirs[@]}
    if [ ${step_dirs_num} -ne ${step_num} ]; then
        handle_error "step_dirs number wrong"
    fi

    pt_files=(`find ${step_dirs[0]} -type f -name '*npy'`)
    pt_files_num=${#pt_files[@]}
    if [ ${pt_files_num} -ne ${weight_grad_num} ]; then
        handle_error "pt_files number wrong"
    fi

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
