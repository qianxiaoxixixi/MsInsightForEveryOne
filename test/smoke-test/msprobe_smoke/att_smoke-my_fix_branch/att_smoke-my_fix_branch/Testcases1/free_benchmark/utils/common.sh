#!/bin/bash

# Requires one parameters: error information
handle_error() {
    duration_time=${duration_time:=0}
    echo "${file_name} fail: $1"
    echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
    exit 1
}

compare_md5() {
    file1=$1
    file2=$2

    input_md5=$(md5sum $file1 | awk '{print $1}')
    baseline_md5=$(md5sum $file2 | awk '{print $1}')
    if [[ $input_md5 != $baseline_md5 ]]; then
        return 1
    fi
    return 0
}

compare_log_count() {
    log1=$1
    log2=$2
    field=$3
    input_count=$(grep $field $log1| wc -l)
    baseline_count=$(grep $field $log2| wc -l)
    if [[ $input_count != $baseline_count ]]; then
        echo $input_count
        echo $baseline_count
        return 1
    fi
    return 0
}

compare_free_benchmark_log() {
    input_log=$1
    baseline_log=$2
    if [ ! -f ${input_log} ]; then
        handle_error "[ERROR] plog.txt is not exist."
    else
        info_feild="\[INFO\]\s\[msprobe\]\sFree\sbenchmark"
        compare_log_count $input_log $baseline_log $info_feild
        if [ $? -eq 1 ];then
            handle_error "${casename} Info count in log check fail"
        fi    
        warning_feild="\[WARNING\]\s\[msprobe\]\sFree\sbenchmark"
        compare_log_count $input_log $baseline_log $warning_feild
        if [ $? -eq 1 ];then
            handle_error "${casename} Wraning count in log check fail"
        fi    
    fi

}

compare_lines() {
    file1=$1
    file2=$2
    input_lines=$(cat $file1 | wc -l)
    baseline_lines=$(cat $file2 | wc -l)
    if [[ $input_lines != $baseline_lines ]]; then
        return 1
    fi
    return 0
}

compare_free_benchmark_result() {
    input_dir=$1
    baseline_dir=$2
    if_drop=$3

    if [ ! -f ${baseline_dir}/free_benchmark.csv ]; then
        return 0
    fi
    # check dump.json
    if [ ! -f ${input_dir}/free_benchmark.csv ]; then
        handle_error "[ERROR] dump json is not exist."
    else
        if [ ${if_drop} -eq 0 ];then
            result=$(python3 -c "import sys;import pandas as pd;\
                x=pd.read_csv(sys.argv[1]+'/free_benchmark.csv').fillna(-1);\
                y=pd.read_csv(sys.argv[2]+'/free_benchmark.csv').fillna(-1);\
                print((x==y).all().all())" ${input_dir} ${baseline_dir})
        else
            result=$(python3 -c "import sys;import pandas as pd;\
                x=pd.read_csv(sys.argv[1]+'/free_benchmark.csv').fillna(-1);\
                x=x.sort_values(by=['step','api_name','rank','output_index']).reset_index();\
                y=pd.read_csv(sys.argv[2]+'/free_benchmark.csv').fillna(-1);\
                y=y.sort_values(by=['step','api_name','rank','output_index']).reset_index();\
                x=x.drop(['max_rel', 'index'], axis=1);\
                y=y.drop(['max_rel', 'index'], axis=1);\
                print((x==y).all().all())" ${input_dir} ${baseline_dir})
        fi
        if [ $? -eq 1 ];then
            handle_error "${casename} run free_benchmark.csv check fail"
        fi
        if [ ! "${result}" = "True" ];then
            handle_error "${casename} free_benchmark.csv check fail"
        fi
    fi
}
