
#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))
source ${cur_dir}/../../Utils/conf.sh
# source ${cur_dir}/src/utils.sh


file_name=$(basename $0 .sh)
log_dir=${output_dir}/ms_api_accuracy_checker_log
log_file=${log_dir}/${file_name}_log.txt
output_path=${output_dir}/ms_api_accuracy_checker_output/test_ms_continue_multi_run_ut
check_script_path=${testcases}/ms_api_accuracy_checker/src/compare_result.py

target_path=${testcases}/ms_api_accuracy_checker/data/pangu_38B_dump_target
dump_json_path=${testcases}/ms_api_accuracy_checker/data/pangu38B_dump.json

csv_path=${testcases}/ms_api_accuracy_checker/data/continue_checker/accuracy_checking_result_20241016152330.csv

# clean
if [ ! -d ${log_dir} ]; then
    mkdir -p ${log_dir}
fi

if [ -f ${log_file} ]; then
    rm -rf ${log_file}
fi

if [ ! -d ${output_path} ]; then
    mkdir -p ${output_path}
else
    rm -rf ${output_path}/*
fi
cp ${testcases}/ms_api_accuracy_checker/data/continue_checker/* ${output_path}/
start_time=$(date "+%s")

# run
# msprobe -f mindspore multi_acc_check -api_info ${dump_json_path} -o ${output_path} -csv_path ${csv_path} -d 0 1 2 3 >> ${log_file}
msprobe -f mindspore multi_acc_check -api_info ${dump_json_path} -o ${output_path} -csv_path ${output_path}/accuracy_checking_result_20241016152330.csv -d 4 5 6 7 >> ${log_file}

end_time=$(date "+%s")
duration_time=$(( ${end_time} - ${start_time} ))
# check
python3 ${check_script_path}  ${target_path} ${output_path} >> ${log_file}

# print
if [ $? -ne 0 ]; then
    echo "${file_name} fail ${duration_time} mindspore"
    exit 1
else
    echo "${file_name} pass ${duration_time} mindspore"
    exit 0
fi


