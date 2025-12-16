#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
source ${cur_dir}/Utils/conf.sh
source ${cur_dir}/Utils/common.sh

PASSWORD="$1"

update_branch() {
    BASE_PROXY="http://90.91.56.202:3128"
    export http_proxy="$BASE_PROXY"
    export https_proxy="$BASE_PROXY"
    export HTTP_PROXY="$BASE_PROXY"
    export HTTPS_PROXY="$BASE_PROXY"
    git config --global http.proxy "$BASE_PROXY"
    git config --global https.proxy "$BASE_PROXY"

    url=https://gitee.com/ascend/mstt.git
    branch=master

    if [ -d ${download_path} ]; then
        rm -rf ${download_path}
    fi

    echo "install tools from ${url} ${branch} to ${download_path}"
    git clone ${url} -b ${branch} ${download_path}

    recover_atat_daily
}

run_and_time() {
    local script_name=$1
    local args=$2
    local start_time=$(date +%s)
    echo "Running ${script_name} ${args}..."
    bash ${script_name} ${args}
    local end_time=$(date +%s)
    local elapsed=$((end_time - start_time))
    echo "Completed ${script_name} in ${elapsed} seconds."
    echo "${script_name} elapsed time: ${elapsed} seconds" >> "${workspace}/task_times.log"
}

main() {
    # 记录开始时间
    start_time=$(date)

    # clean workspace
    if [ -d ${workspace} ]; then
        rm -rf ${output_dir}/*
    else
        mkdir ${workspace}
    fi

    update_branch

    # init env
    init_env_daily

    # 记录任务执行时间
    run_and_time "${testcases}/run_ms_free_benchmark.sh" 1
    run_and_time "${testcases}/run_ms_overflow_check.sh" 1
    run_and_time "${testcases}/run_ms_set_dump.sh" 1
    run_and_time "${testcases}/run_msprobe_ms_dump.sh" 1
    run_and_time "${testcases}/run_pt_dump.sh" 2
    run_and_time "${testcases}/run_free_benchmark.sh" 1
    # run_and_time "${testcases}/run_pt_compare.sh"
    # run_and_time "${testcases}/run_api_accuracy_checking.sh"
    # run_and_time "${testcases}/run_visualization.sh"
    # run_and_time "${testcases}/run_grad_probe.sh"
    # run_and_time "${testcases}/run_online_dispatch.sh"
    run_and_time "${testcases}/run_msprobe_ms_grad_probe.sh" 1
    run_and_time "${testcases}/run_ms_api_accuracy_checker.sh" 1

    filter_and_print ${output_dir}/plog.txt | tee ${smoke_log}
    zip_and_print ${output_dir}

    echo "Script started at: ${start_time}"
    # Print the current time
    echo "Script completed at: $(date)"

    # 打印每个任务的耗时
    echo "Task Execution Times:"
    cat "${workspace}/task_times.log"
}

main

echo "smoke_log: ${smoke_log}"
echo "after_move_smoke_log: ${after_move_smoke_log}"
rm -rf /home/maoyanlong/ascend/log/debug/plog/
rm -rf /home/maoyanlong/ascend/log/debug/device-*
rm -rf ~/ascend/log/run/plog/plog-*
mv ${workspace}/output_* /data00/maoyanlong/WorkSpace1/
mv ${workspace}/smoke_* /data00/maoyanlong/WorkSpace1/
