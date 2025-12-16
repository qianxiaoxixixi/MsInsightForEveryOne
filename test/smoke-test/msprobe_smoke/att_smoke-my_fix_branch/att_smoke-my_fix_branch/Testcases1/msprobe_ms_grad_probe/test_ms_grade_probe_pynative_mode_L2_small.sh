#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))
source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/src/utils.sh

level=2
mode="pynative"
csv_row_num=9

file_name=$(basename $0 .sh)
log_dir=${output_dir}/ms_grad_probe_log
log_file=${log_dir}/${file_name}_log.txt
dump_path=${output_dir}/ms_grad_probe_output
dump_config_path=${testcases}/msprobe_ms_grad_probe/json/L${level}_config.json
resnet_path=${testcases}/msprobe_ms_grad_probe/src/resnet_${mode}.py
check_script_path=${testcases}/msprobe_ms_grad_probe/src/check_grad_summary.py
checked_grad_summary_csv_path=${dump_path}/rank0/grad_summary_0.csv


# clean
if [ ! -d ${log_dir} ]; then
    mkdir -p ${log_dir}
fi

if [ -f ${log_file} ]; then
    rm -rf ${log_file}
fi

if [ ! -d ${dump_path} ]; then
    mkdir -p ${dump_path}
else
    rm -rf ${dump_path}/*
fi

start_time=$(date "+%s")
# replace
replace_json ${dump_path} ${dump_config_path} >> ${log_file}

# run
python3 ${resnet_path} ${level} >> ${log_file}

end_time=$(date "+%s")
duration_time=$(( ${end_time} - ${start_time} ))
# check
python3 ${check_script_path} ${level} ${mode} ${checked_grad_summary_csv_path} ${csv_row_num} >> ${log_file}

# print
if [ $? -ne 0 ]; then
    echo "${file_name} fail ${duration_time} mindspore"
    exit 1
else
    echo "${file_name} pass ${duration_time} mindspore"
    exit 0
fi
