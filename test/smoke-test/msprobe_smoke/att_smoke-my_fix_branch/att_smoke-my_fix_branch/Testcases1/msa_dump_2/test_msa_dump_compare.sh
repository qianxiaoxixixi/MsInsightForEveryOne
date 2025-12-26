#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))
output_dir=$1
task=$2
check_compare_files()
{
    local output_dir=$1
    local expected_count=$2
    local error_message=$3
    local file_name=$4
    local file_count=$(find "$output_dir" -type f | wc -l)
    if [ "$file_count" -ne "$expected_count" ]; then
        echo "${file_name} fail: [ERROR] $error_message"
        echo "${file_name} fail ${duration_time}s"
        return 1
    fi
    return 0
}

compare_check() {
    local compare_output=$1
    local duration_time=$2

    if [[ "$compare_output" == *"msa_dump_compare_md5"* ]]; then
        check_compare_files "${compare_output}" 3 "Compare md5 interrupted." "test_msa_dump_compare_md5"
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi

    if [[ "$compare_output" == *"msa_dump_compare_statistics"* ]]; then
        check_compare_files "${compare_output}" 17 "Compare statistics interrupted." "test_msa_dump_compare_statistics"
        if [ $? -ne 0 ]; then
            return 1
        fi
    fi


    return 0
}


testcase_result_dir="${output_dir}/msa_dump_compare/${task}"
if [ ! -d ${testcase_result_dir} ]; then
    mkdir -p ${testcase_result_dir}
else
    rm -rf ${testcase_result_dir}/*
fi

start_time_compare=$(date '+%s')
if [[ ${task} == "test_msa_dump_compare_md5" ]]; then
    python3 ${cur_dir}/src/${task}.py "${output_dir}/msa_dump/test_msa_dump_compare_md5" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
elif [[ ${task} == "test_msa_dump_compare_statistics" ]]; then
    python3 ${cur_dir}/src/${task}.py "${output_dir}/msa_dump/test_msa_dump_compare_statistics" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
fi
end_time_compare=$(date '+%s')
duration_time=$(( ${end_time_compare} - ${start_time_compare} ))
compare_check ${testcase_result_dir} ${duration_time}
if [ $? -eq 0 ]; then
    echo "$task pass ${duration_time}s"
fi
# main() {
#     testcase_result_dir="${output_dir}/msa_dump_compare/${task}"
#     echo "${testcase_result_dir}"
#     if [ ! -d ${testcase_result_dir} ]; then
#         mkdir -p ${testcase_result_dir}
#     else
#         rm -rf ${testcase_result_dir}/*
#     fi

#     start_time_compare=$(date '+%s')
#     if [[ ${task} == "test_msa_dump_compare_md5" ]]; then
#         python3 ${cur_dir}/src/${task}.py "${output_dir}/msa_dump/test_msa_dump_compare_md5" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
#     elif [[ ${task} == "test_msa_dump_compare_statistics" ]]; then
#         python3 ${cur_dir}/src/${task}.py "${output_dir}/msa_dump/test_msa_dump_compare_statistics" "${testcase_result_dir}" >> ${testcase_result_dir}/plog.txt
#     fi
#     end_time_compare=$(date '+%s')
#     duration_time=$(( ${end_time_compare} - ${start_time_compare} ))
#     compare_check ${testcase_result_dir} ${duration_time}
#     if [ $? -eq 0 ]; then
#         echo "$task pass ${duration_time}s"
#     fi
# }


# main
