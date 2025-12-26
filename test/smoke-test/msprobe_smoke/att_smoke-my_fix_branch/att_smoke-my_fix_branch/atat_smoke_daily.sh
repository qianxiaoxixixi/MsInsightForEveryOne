#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
source ${cur_dir}/Utils/conf.sh
source ${cur_dir}/Utils/common.sh

PASSWORD="$1"

update_branch() {
    # export http_proxy="http://p_atlas:proxy%40123@90.91.56.202:3128"
    # export https_proxy="http://p_atlas:proxy%40123@90.91.56.202:3128"
    # export no_proxy=127.0.0.1,.huawei.com,localhost,local,.local
    BASE_PROXY="http://90.91.56.202:3128"
    export http_proxy="$BASE_PROXY"
    export https_proxy="$BASE_PROXY"
    export HTTP_PROXY="$BASE_PROXY"
    export HTTPS_PROXY="$BASE_PROXY"
    git config --global http.proxy "$BASE_PROXY"
    git config --global https.proxy "$BASE_PROXY"

    url=https://gitcode.com/Ascend/mstt.git
    branch=master

    if [ -d ${download_path} ]; then
        # rm -rf ${download_path}
        if [[ -n "$download_path" && ${#download_path} -gt 2 && -e "$download_path" ]]; then
            rm -rf ${download_path}
        fi
    fi

    # git download
    echo "install tools from ${url} ${branch} to ${download_path}"
    git clone ${url} -b ${branch} ${download_path}

    if [ -d ${download_path}/debug ]; then
        recover_atat_daily
    fi

}


main() {
    # 记录开始时间
    start_time=$(date)

    # clean workspace
    if [ -d ${workspace} ];then
        # rm -rf ${output_dir}/*
        if [[ -n "$output_dir" && ${#output_dir} -gt 2 && -e "$output_dir" ]]; then
            rm -rf ${output_dir}/*
        fi
    else
        mkdir ${workspace}
    fi

    update_branch

    # init env
    init_env_daily
    if [ -d ${download_path}/debug ]; then
        compile_pkg
    fi
    echo ####################################
    # 調用安裝函數
    pkill_python "$PASSWORD"
    # bash ${testcases}/run_ms_38BV3.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_msprobe_ms_dump.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_pt_dump.sh 4
    pkill_python "$PASSWORD"
    # bash ${testcases}/run_overflow_check.sh
    bash ${testcases}/run_free_benchmark.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_pt_compare.sh 3
    pkill_python "$PASSWORD"
    bash ${testcases}/run_api_accuracy_checking.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_visualization.sh
    pkill_python "$PASSWORD"
    bash ${testcases}/run_ms_set_dump.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_ms_overflow_check.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_ms_free_benchmark.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_grad_probe.sh
    pkill_python "$PASSWORD"
    bash ${testcases}/run_online_dispatch.sh
    pkill_python "$PASSWORD"
    bash ${testcases}/run_msprobe_ms_compare.sh
    pkill_python "$PASSWORD"
    bash ${testcases}/run_ms_api_accuracy_checker.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_msprobe_ms_grad_probe.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_msmonitor.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_ms_acl_dump.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_msa_dump_2.sh 4
    pkill_python "$PASSWORD"
    bash ${testcases}/run_msa_overflow_check.sh 4


    filter_and_print ${output_dir}/plog.txt | tee ${smoke_log}
    zip_and_print ${output_dir}

    echo "Script started at: ${start_time}"
    # Print the current time
    echo "Script completed at: $(date)"
}

main
echo "smoke_log: ${smoke_log}"
echo "after_move_smoke_log: ${after_move_smoke_log}"
rm -rf /home/maoyanlong/ascend/log/debug/plog/
rm -rf /home/maoyanlong/ascend/log/debug/device-*
rm -rf ~/ascend/log/run/plog/plog-*
mv ${workspace}/output_* /data00/maoyanlong/WorkSpace1/
mv ${workspace}/smoke_* /data00/maoyanlong/WorkSpace1/

