#!/bin/bash

cur_dir=$(dirname $(readlink -f $0))
# 记录开始时间
start_time=$(date)


source ${cur_dir}/Utils/conf.sh
source ${cur_dir}/Utils/config.sh
source ${cur_dir}/Utils/common.sh

# 定义锁文件路径
LOCKFILE="/tmp/atat_smoke_jenkins_bak.lock"

# 尝试获取锁（将会阻塞，直到获取到锁）
{
    echo "Waiting for the lock to become available..."
    flock 200  # 等待获取锁，直到锁可用




    while getopts ':g:b:m:' OPT; do
        case ${OPT} in
            "g")  # tools URL
                git_url="$OPTARG"
                ;;
            "b")  # tools 分支名
                branch="$OPTARG"
                ;;
            "m")  # 冒烟选择模块
                modules="$OPTARG"
                ;;
            *)
                break
                ;;
        esac
    done

    update_branch() {
        export http_proxy="http://p_atlas:proxy%40123@90.91.56.202:3128"
        export https_proxy="http://p_atlas:proxy%40123@90.91.56.202:3128"
        export no_proxy=127.0.0.1,.huawei.com,localhost,local,.local

        url=$1
        branch=$2

        if [ -d ${download_path} ]; then
            # rm -rf ${download_path}
            if [[ -n "$download_path" && ${#download_path} -gt 2 && -e "$download_path" ]]; then
                rm -rf ${download_path}
            fi
        fi

        # git download
        if echo $url | grep -q "https://gitee.com/ascend"; then
            echo "install tools from ${url} ${branch}"
            git clone ${url} -b ${branch} ${download_path}
        # git clone https://gitee.com/wqc01202410/att.git -b master ${download_path}
        elif echo $url | grep -q "pull"; then
            # pull/1127/head:pr_1127
            echo "install tools accroding fetch method"
            git clone -b ${branch} https://gitee.com/ascend/mstt.git ${download_path}

            cd ${download_path}
            git fetch https://gitee.com/ascend/mstt.git $url

            pr_id=`echo $url | awk -F "head:" '{print $2}'`
            echo 'get pr_id: ${pr_id}'
            git merge $pr_id -X ours -m "Merging PR ID $pr_id"
        else
            echo "Input command is incorrect."
            exit 1
        fi

        # recover atat
        recover_atat_daily
    }


    main() {

        #for i in $(seq 3600 -1 1); do
        #    printf "\r等待中，剩余时间：%d秒" "$i"
        #    sleep 1
        #done
        #echo -e "\n完成等待，继续执行..."

        local git_url=$1
        local branch=$2
        local modules=$3

        # clean workspace
        if [ -d ${workspace} ];then
            # rm -rf ${output_dir}/*
            if [[ -n "$output_dir" && ${#output_dir} -gt 2 && -e "$output_dir" ]]; then
                rm -rf ${output_dir}/*
            fi
        else
            mkdir ${workspace}
        fi
        # update_branch ${git_url} ${branch}

        # init env
        # init_env_daily
        # compile_pkg
        # 調用安裝函數

        if [ "api_accuracy_checking" == "${modules}" ]; then
            bash ${testcases}/run_api_accuracy_checking.sh 4
        elif [ "pt_compare" == "${modules}" ]; then
            bash ${testcases}/run_pt_compare.sh 3
            bash ${testcases}/run_visualization.sh
        elif [ "overflow_check" == "${modules}" ]; then
            bash ${testcases}/run_overflow_check.sh
        elif [ "free_benchmark" == "${modules}" ]; then
            bash ${testcases}/run_free_benchmark.sh 4
        elif [ "all_pt_dump" == "${modules}" ]; then
            bash ${testcases}/run_pt_dump.sh 4
        elif [ "pt_dump" == "${modules}" ]; then
            bash ${testcases}/run_pt_dump.sh 1
        elif [ "msprobe_ms_dump" == "${modules}" ]; then
            bash ${testcases}/run_msprobe_ms_dump.sh 2
        elif [ "dump" == "${modules}" ]; then
 	    bash ${testcases}/run_msprobe_ms_dump.sh 2
	    bash ${testcases}/run_pt_dump.sh 2
	    #bash ${testcases}/run_overflow_check.sh 2
        elif [ "msprobe_ms_dump_all" == "${modules}" ]; then
            bash ${testcases}/run_msprobe_ms_dump.sh 4
        elif [ "visualization" == "${modules}" ]; then
            bash ${testcases}/run_visualization.sh
        elif [ "ms_set_dump" == "${modules}" ]; then
            bash ${testcases}/run_ms_set_dump.sh 2
        elif [ "ms_overflow_check" == "${modules}" ]; then
            bash ${testcases}/run_ms_overflow_check.sh 2
        elif [ "ms_free_benchmark" == "${modules}" ]; then
            bash ${testcases}/run_ms_free_benchmark.sh 2
        elif [ "common" == "${modules}" ]; then
            bash ${testcases}/run_ms_free_benchmark.sh 1
            bash ${testcases}/run_ms_overflow_check.sh 1
            bash ${testcases}/run_ms_set_dump.sh 1
            bash ${testcases}/run_msprobe_ms_dump.sh 1
            bash ${testcases}/run_pt_dump.sh 1
            bash ${testcases}/run_free_benchmark.sh 1
            bash ${testcases}/run_pt_compare.sh 1
            bash ${testcases}/run_api_accuracy_checking.sh 1
            bash ${testcases}/run_visualization.sh
            bash ${testcases}/run_grad_probe.sh
            bash ${testcases}/run_online_dispatch.sh
            bash ${testcases}/run_msprobe_ms_grad_probe.sh 1
            bash ${testcases}/run_ms_api_accuracy_checker.sh 1
            bash ${testcases}/run_msmonitor.sh  1
        elif [ "larger_common" == "${modules}" ]; then
            bash ${testcases}/run_ms_free_benchmark.sh 2
            bash ${testcases}/run_ms_overflow_check.sh 2
            bash ${testcases}/run_ms_set_dump.sh 2
            bash ${testcases}/run_msprobe_ms_dump.sh 2
            bash ${testcases}/run_pt_dump.sh 2
            bash ${testcases}/run_free_benchmark.sh 2
            bash ${testcases}/run_pt_compare.sh 2
            bash ${testcases}/run_api_accuracy_checking.sh 2
            bash ${testcases}/run_visualization.sh
            bash ${testcases}/run_grad_probe.sh
            bash ${testcases}/run_online_dispatch.sh
            bash ${testcases}/run_msprobe_ms_grad_probe.sh 2
            #bash ${testcases}/run_ms_api_accuracy_checker.sh 2
            bash ${testcases}/run_msmonitor.sh  2
        elif [ "test" == "${modules}" ]; then
            bash ${testcases}/run_msprobe_ms_dump.sh 1
        elif [ "ms_compare" == "${modules}" ]; then
            bash ${testcases}/run_msprobe_ms_compare.sh
            bash ${testcases}/run_visualization.sh
        elif [ "grad_probe" == "${modules}" ]; then
            bash ${testcases}/run_grad_probe.sh
        elif [ "online_dispatch" == "${modules}" ]; then
            bash ${testcases}/run_online_dispatch.sh
        elif [ "msmonitor" == "${modules}" ]; then
            bash ${testcases}/run_msmonitor.sh  4
        elif [ "msprobe_ms_grad_probe" == "${modules}" ]; then
            bash ${testcases}/run_msprobe_ms_grad_probe.sh 2
        elif [ "ms_api_accuracy_checker" == "${modules}" ]; then
            bash ${testcases}/run_ms_api_accuracy_checker.sh 2
        fi

        filter_and_print ${output_dir}/plog.txt | tee ${smoke_log}
        zip_and_print ${output_dir}

        echo "Script started at: ${start_time}"
        # Print the current time
        echo "Script completed at: $(date)"
    }

    # run_nums=$(ps -ef | grep "atat_smoke_jenkins.sh" | wc -l)
    # echo $run_nums
    # if [ "$run_nums" -gt 3 ]; then
    #     echo "There is another one run atat_smoke_jenkins.sh"
    #     exit 1
    # fi

    main ${git_url} ${branch} ${modules}

    echo "smoke_log: ${smoke_log}"
    echo "after_move_smoke_log: ${after_move_smoke_log}"

    rm -rf ~/ascend/log/debug/plog/
    rm -rf ~/ascend/log/debug/device-*
    mv ${workspace}/output_* /data00/maoyanlong/WorkSpace1/
    mv ${workspace}/smoke_* /data00/maoyanlong/WorkSpace1/

} 200>$LOCKFILE

