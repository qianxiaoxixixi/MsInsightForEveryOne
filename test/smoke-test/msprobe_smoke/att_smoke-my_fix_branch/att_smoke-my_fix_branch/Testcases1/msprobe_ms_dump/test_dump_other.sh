#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))


# export ASCEND_SLOG_PRINT_TO_STDOUT=0
# export ASCEND_GLOBAL_LOG_LEVEL=0
source ${cur_dir}/../../Utils/conf.sh
source ${cur_dir}/utils/common.sh
source ${cur_dir}/../../Utils/common.sh

# init_env_daily
# conda activate pytorch21

# need input the config.json
version=$1
if [[ $version == "new" ]]; then
  export PYTHONPATH=/home/maoyanlong/att_smoke_new/Testcases1/ms_overflow_check/mstt_new/debug/accuracy_tools/:$PYTHONPATH
else
  export PYTHONPATH=/home/maoyanlong/att_smoke_new/Testcases1/ms_overflow_check/mstt/debug/accuracy_tools/:$PYTHONPATH
fi

json_name=$2
npu_id=${3:-4}  # 如果没有传入 NPU ID，则默认值为 4
output_override=$4

main() {

    file_name=$(basename $json_name .json)
    testcase_result_dir="${output_dir}/ms_dump/${file_name}"
    if [ -n "$output_override" ]; then
        testcase_result_dir="$output_override"
    fi

    #testcase_result_dir="${cur_dir}/baseline/${file_name}"
    #生成基线数据
    # mkdir -p ${testcase_result_dir}
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    mkdir -p ${testcase_result_dir}


    json_path=${cur_dir}/json/${file_name}.json
    # sed -i "s#\"dump_path\":\"[^\"]*\"#\"dump_path\":\"${testcase_result_dir}\"#" $json_path
    sed -i "s#\"dump_path\".*#\"dump_path\":\"${testcase_result_dir}\",#" $json_path

    #将dump路径进行替换

    #开启确定性计算
    export HCCL_DETERMINISTIC=true
    export ASCEND_LAUNCH_BLOCKING=1
    start_time=$(date "+%s")
    if [[ ${json_name} == Larger* ]]; then
        # conda activate pytorch21_copy
        # mindspore_init_env_daily
    #运行大模型
    	echo "Running large1 model with parameter: ${json_name}"
        export PYTHONPATH=${cur_dir}/../../Model/dynamic-parallel:$PYTHONPATH
        cp ${json_path}  ${cur_dir}/../../Model/dynamic-parallel/mindformers/experimental/pangu_demo/config.json
        cd ${cur_dir}/../../Model/dynamic-parallel/mindformers/experimental/pangu_demo/
        # 启动删除npy文件进程
        # delete_dump_files &
        # delete_pid=$!
        echo "Current directory: $(pwd)"

# {
        # # 定义脚本路径
        # script_path="${cur_dir}/../../Model/dynamic-parallel/mindformers/experimental/pangu_demo/run_pp.sh"
        # # 查找当前的 master_port 值并加4
        # current_port=$(grep -- '--master_port=' "$script_path" | awk -F '--master_port=' '{print $2}' | awk '{print $1}')

        # # 检查是否只获得了一个有效的端口号
        # if [[ -z "$current_port" || ! "$current_port" =~ ^[0-9]+$ ]]; then
        #     echo "Error: Failed to extract a valid current_port. Found: $current_port"
        #     exit 1
        # fi
        # # 计算新端口号
        # new_port=$((current_port + 10))

        # # 检查端口号是否正确解析
        # if ! [[ "$new_port" =~ ^[0-9]+$ ]]; then
        #     echo "Error: Failed to calculate the new port number. Current port: $current_port, New port: $new_port"
        #     exit 1
        # fi

        # # 修改脚本中的 master_port 值
        # sed -i "s/--master_port=$current_port/--master_port=$new_port/" "$script_path"

        # echo "Updated master_port to $new_port in $script_path"

# }

        bash run_pp.sh
        wait 
        echo "123test"
        # kill $delete_pid
        cp -r  ${cur_dir}/../../Model/dynamic-parallel/mindformers/experimental/pangu_demo/pp_log/  ${testcase_result_dir}
        
        base_line_dir=${cur_dir}/baseline/${file_name}

        validate_output_directory ${json_path} ${testcase_result_dir} ${cur_dir}/baseline/${file_name}


        pkill_python maoyanlong

	
    elif [[ ${json_name} == exception* ]]; then
        # 处理异常情况
        echo "Handling exception case with parameter: ${json_name}"
        cp ${json_path}  ${cur_dir}/src/config.json
        python ${cur_dir}/src/test_primitive_hook_object_for.py
	#python ${cur_dir}/src/test_primitive_hook_object_for.py >> ${testcase_result_dir}/plog.txt 2>&1
        # 检查 Python 脚本的退出状态码

        if [ $? -eq 0 ]; then
            end_time=$(date "+%s")
            duration_time=$(( ${end_time} - ${start_time} ))
            # 如果脚本运行成功，则输出错误信息
            handle_error "exception ${file_name} script failed."
        fi
    else
    #运行小模型
        echo "Running small model with parameter: ${json_name}"
        # cp ${json_path}  ${cur_dir}/src/config.json
        cp ${json_path} ${cur_dir}/src/config${npu_id}.json
        # 修改 Python 脚本中的设备 ID
        sed -i "s/context.set_context(device_id=[0-9]\+/context.set_context(device_id=${npu_id}/" ${cur_dir}/src/test_primitive_hook_object_for_mul.py
        # sed -i "s/context.set_context(device_id=[0-9]\+/context.set_context(device_id=${npu_id}/" ${cur_dir}/src/test_primitive_hook_object_for.py
        
        #python ${cur_dir}/src/test_primitive_hook_object_for.py 
	    # python ${cur_dir}/src/test_primitive_hook_object_for.py >> ${testcase_result_dir}/plog.txt
	echo "begin at $(date)"
        python ${cur_dir}/src/test_primitive_hook_object_for_mul.py ${npu_id} >> ${testcase_result_dir}/plog.txt
	echo "end at $(date)"
        base_line_dir=${cur_dir}/baseline/${file_name}
	    compare_dump ${json_path}  ${cur_dir}/baseline/${file_name} ${testcase_result_dir}
    fi



    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    #compare_dump ${cur_dir}/baseline/${file_name} ${testcase_result_dir}
    # validate_output_directory ${json_path} ${testcase_result_dir}

    # rm -rf ${testcase_result_dir}/*

    echo "${file_name} pass ${duration_time} mindspore"
}

main

# init_env_daily
