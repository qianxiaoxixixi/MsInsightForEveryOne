#!/bin/bash

# Requires one parameters: error information
handle_error() {
    echo "$1"
    duration_time=${duration_time:=0}
    echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
    echo "${file_name} fail: $1"
    exit 1
}

# Requires two parameters: accuracy_checking_result_num, accuracy_checking_details_num
verify_ut_csv() {
    sed -i '1s/^\xEF\xBB\xBF//' ${testcase_result_dir}/accuracy_checking_result_*.csv
    sed -i '1s/^\xEF\xBB\xBF//' ${testcase_result_dir}/accuracy_checking_details_*.csv
    accuracy_checking_details_expected_header="API Name,Bench Dtype,DEVICE Dtype,Shape,余弦相似度,最大绝对误差,双百指标,双千指标,双万指标,二进制一致错误率,误差均衡性,均方根误差,小值域错误占比,相对误差最大值,相对误差平均值,inf/nan错误率,相对误差错误率,绝对误差错误率,ULP误差最大值,ULP误差平均值,ULP误差大于阈值占比,Status,Message"
    accuracy_checking_result_expected_header="API name,Forward Test Success,Backward Test Success,Message"

    if [ ! -f ${testcase_result_dir}/accuracy_checking_result_*.csv ]; then
        handle_error "[ERROR] accuracy_checking_result.csv is not exist."
    fi
    accuracy_checking_result_actual_head=$(head -n 1 ${testcase_result_dir}/accuracy_checking_result_*.csv | tr -d '\r')
    if [ "$accuracy_checking_result_expected_header" != "$accuracy_checking_result_actual_head" ]; then
        handle_error "[ERROR] accuracy_checking_result.csv header abnormality"
    fi
    accuracy_checking_result_num=$(cat ${testcase_result_dir}/accuracy_checking_result_*.csv |wc -l)
    if  [[ accuracy_checking_result_num -ne $1 ]]; then
        handle_error "[ERROR] the number ${accuracy_checking_result_num} of api in accuracy_checking_result.csv is incorrect."
    fi
    if [ ! -f ${testcase_result_dir}/accuracy_checking_details_*.csv ]; then
        handle_error "[ERROR] accuracy_checking_details.csv is not exist."
    fi
    #accuracy_checking_details_actual_head=$(head -n 1 ${testcase_result_dir}/accuracy_checking_details_*.csv | tr -d '\r')
    #if [ "$accuracy_checking_details_expected_header" != "$accuracy_checking_details_actual_head" ]; then
    #    handle_error "[ERROR] accuracy_checking_details.csv header abnormality"
    #fi
    accuracy_checking_details_num=$(cat ${testcase_result_dir}/accuracy_checking_details_*.csv |wc -l)
    if  [[ accuracy_checking_details_num -ne $2 ]]; then
        handle_error "[ERROR] the number ${accuracy_checking_details_num} of api in accuracy_checking_details.csv is incorrect."
    fi
}

# Requires three parameters: forward_info.json num, backward_info.json num, stack_info.json num
verify_dump_json() {
    if [ ! -f ${testcase_result_dir}/step1/forward_info*.json ]; then
        handle_error "[ERROR] forward_info json is not exist."
    fi
    forward_info_count=$(cat ${testcase_result_dir}/step1/forward_info*.json |wc -l)
    if  [[ forward_info_count -ne $1 ]]; then
        handle_error "[ERROR] the number ${forward_info_count} of api in forward_info.json is incorrect."
    fi
    if [ ! -f ${testcase_result_dir}/step1/backward_info*.json ]; then
        handle_error "[ERROR] backward_info json not exist."
    fi
    backward_info_count=$(cat ${testcase_result_dir}/step1/backward_info*.json |wc -l)
    if  [[ backward_info_count -ne $2 ]]; then
        handle_error "[ERROR] the number ${backward_info_count} of api in backward_info.json is incorrect."
    fi
    if [ ! -f ${testcase_result_dir}/step1/stack_info*.json ]; then
        handle_error "[ERROR] stack_info json not exist."
    fi
    stack_info_count=$(cat ${testcase_result_dir}/step1/stack_info*.json |wc -l)
    if  [[ stack_info_count -ne $3 ]]; then
        handle_error "[ERROR] the number of ${stack_info_count} api in stack_info.json is incorrect."
    fi
}

# Requires two parameters: api_precision_compare_result_num, api_precision_compare_details_num
verify_compare_csv() {
    sed -i '1s/^\xEF\xBB\xBF//' ${testcase_result_dir}/api_precision_compare_result_*.csv
    sed -i '1s/^\xEF\xBB\xBF//' ${testcase_result_dir}/api_precision_compare_details_*.csv
    api_precision_compare_details_expected_header="API Name,小值域错误比值,小值域判定结果,均方根误差比值,均方根误差判定结果,相对误差最大值比值,相对误差最大值判定结果,相对误差平均值比值,相对误差平均值判定结果,误差均衡性比值,误差均衡性判定结果,inf/nan错误率,inf/nan判定结果,相对误差错误率,相对误差判定结果,绝对误差错误率,绝对误差判定结果,二进制一致错误率,二进制一致错误率判定结果,ULP误差平均值,ULP误差大于阈值占比,ULP误差大于阈值占比比值,ULP误差判定结果,双千指标,双千指标判定结果,比对结果,比对算法,Message"
    api_precision_compare_result_expected_header="API Name,Forward Test Success,Backward Test Success,Message"

    if [ ! -f ${testcase_result_dir}/api_precision_compare_result_*.csv ]; then
        handle_error "[ERROR] api_precision_compare_result.csv is not exist."
    fi
    api_precision_compare_result_actual_head=$(head -n 1 ${testcase_result_dir}/api_precision_compare_result_*.csv | tr -d '\r')

    if [ "$api_precision_compare_result_expected_header" != "$api_precision_compare_result_actual_head" ]; then
        handle_error "[ERROR] api_precision_compare_result.csv header abnormality"
    fi
    api_precision_compare_result_num=$(cat ${testcase_result_dir}/api_precision_compare_result_*.csv |wc -l)
    if  [[ api_precision_compare_result_num -ne $1 ]]; then
        handle_error "[ERROR] the number ${api_precision_compare_result_num} of api in api_precision_compare_result.csv is incorrect."
    fi
    if [ ! -f ${testcase_result_dir}/api_precision_compare_details_*.csv ]; then
        handle_error "[ERROR] api_precision_compare_details.csv is not exist."
    fi
    #api_precision_compare_details_actual_head=$(head -n 1 ${testcase_result_dir}/api_precision_compare_details_*.csv | tr -d '\r')
    #if [ "$api_precision_compare_details_expected_header" != "$api_precision_compare_details_actual_head" ]; then
    #    handle_error "[ERROR] api_precision_compare_details.csv header abnormality"
    #fi
    api_precision_compare_details_num=$(cat ${testcase_result_dir}/api_precision_compare_details_*.csv |wc -l)
    if  [[ api_precision_compare_details_num -ne $2 ]]; then
        handle_error "[ERROR] the number ${api_precision_compare_details_num} of api in api_precision_compare_details.csv is incorrect."
    fi
}
