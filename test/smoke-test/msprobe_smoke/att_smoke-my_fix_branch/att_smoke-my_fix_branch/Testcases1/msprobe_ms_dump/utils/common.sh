#!/bin/bash
# 设置阈值
COSINE_THRESHOLD=0.99     # 设置余弦相似度的阈值
REL_ERROR_THRESHOLD=0.5   # 设置相对误差的阈值

# Requires one parameters: error information
handle_error() {
    start_time=${start_time:=0}
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    # duration_time=${duration_time:=0}
#
    echo "${file_name} FAIL: $1"
    echo "${file_name} fail ${duration_time} mindspore"
    
    echo "比较目录 '${base_line_dir}' 和 '${testcase_result_dir}' 的差异："
    diff_output=$(diff -rq "${base_line_dir}" "${testcase_result_dir}")
    if [ -n "$diff_output" ]; then
        echo "$diff_output"
    else
        echo "没有发现差异。"
    fi

    echo "单独运行此用例命令为：
    source ${cur_dir}/../../Utils/conf.sh
    source ${cur_dir}/utils/common.sh
    source ${cur_dir}/../../Utils/common.sh
    init_env_daily
    bash ${cur_dir}/test_dump.sh ${json_name}"
    exit 1
#
}

compare_md5() {
    file1=$1
    file2=$2

    input_md5=$(md5sum $file1 | awk '{print $1}')
    baseline_md5=$(md5sum $file2 | awk '{print $1}')
    if [[ $input_md5 != $baseline_md5 ]]; then
        echo "[ERROR] MD5 checksum does not match for files ${file1} and ${file2}."
        return 1
    fi
    echo "[INFO] MD5 checksum matches for files ${file1} and ${file2}."
    return 0
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

# 比較小模型方法
compare_dump() {
    local json_path=$1
    input_dir=$2
    baseline_dir=$3
    # 提取 step 和 rank
    steps=$(jq -r '.step[]' "${json_path}")
    ranks=$(jq -r '.rank[]' "${json_path}")
    # 如果 steps 为空，则设置默认值为 0
    if [ -z "$steps" ]; then
        steps="0"
        echo "[WARNING] No steps found in ${json_path}. Defaulting to step=0."
    fi
    echo "[WARNING] cur_dir : ${cur_dir}"
    for step in ${steps}; do
        for rank in ${ranks}; do
            check_dump_json "${input_dir}" "${baseline_dir}" "min_model" "${step}"
        done
    done

}


check_dump_json() {
    input_dir=$1
    baseline_dir=$2
    rank_id=$3
    step_id=$4

    echo "Current directory: $(pwd)"
    # 获取当前脚本的目录
    script_dir=$(dirname $(readlink -f $0))
    echo "Script directory: ${script_dir}"
    echo "Current directory: $(pwd)"
    echo "cur_dir : ${cur_dir}"
    echo "[WARNING] 2cur_dir : ${cur_dir}"
    # 切换到脚本所在的目录
    cd "${script_dir}" || handle_error "[ERROR] Unable to change to script directory: ${script_dir}"

    # 如果 rank_id 为 "min_model"，使用 "rank"
    if [ "$rank_id" == "min_model" ]; then
        rank_path="rank"
    elif [ -n "$rank_id" ]; then
        rank_path="rank${rank_id}"
    else
        rank_path="rank0"
    fi
    # 如果提供了 step_id，使用 step{step_id}，否则使用 step0
    if [ -n "$step_id" ]; then
        step_path="step${step_id}"
    else
        step_path="step0"
    fi

    # 检查 dump.json 是否存在
    if [ ! -f ${input_dir}/${step_path}/${rank_path}/dump.json ]; then
        handle_error "[ERROR] Larger dump.json is not exist in ${input_dir}/${step_path}/${rank_path}."
    else
        # 比较 dump.json 的 MD5 值
        compare_md5 ${input_dir}/${step_path}/${rank_path}/dump.json ${baseline_dir}/${step_path}/${rank_path}/dump.json

        if [ $? -eq 1 ]; then
            echo "[WARNING] MD5 check inconformity, comparing cosine similarity and relative error..."
            
        # 使用 compare.py 比较两个文件的余弦相似度与相对误差，并保存结果到指定目录
            echo "Executing: python3 ${cur_dir}/utils/compare.py ${baseline_dir}/${step_path}/${rank_path}/dump.json ${input_dir}/${step_path}/${rank_path}/dump.json ${temp_dir}"
            temp_dir="${input_dir}/${step_path}/${rank_path}"
            python3 ${cur_dir}/utils/compare.py ${baseline_dir}/${step_path}/${rank_path}/dump.json ${input_dir}/${step_path}/${rank_path}/dump.json ${temp_dir}
            
            # 读取 compare_result.txt 中的相似度与相对误差
            # 读取 temp_dir 中的 comparison_table.csv
            while IFS=',' read -r operator rel_error cosine_similarity; do
                # 跳过表头
                if [ "$operator" == "Operator" ]; then
                    continue
                fi

                # 检查是否超过了阈值
                if (( $(echo "$cosine_similarity < $COSINE_THRESHOLD" | bc -l) )) || \
                (( $(echo "$rel_error > $REL_ERROR_THRESHOLD" | bc -l) )); then
                    handle_error "[ERROR] ${operator} fails the comparison: Relative Error = ${rel_error}, Cosine Similarity = ${cosine_similarity}."
                fi
            done < "${temp_dir}/comparison_table.csv"
        fi
        if [ $? -eq 1 ]; then
            handle_error "[ERROR] Larger dump.json check fail in ${baseline_dir}/${step_path}/${rank_path}/dump.json and ${input_dir}/${step_path}/${rank_path}/dump.json."
        fi
    fi

    # check construct.json
    if [ ! -f ${input_dir}/${step_path}/${rank_path}/construct.json ]; then
        handle_error "[ERROR] construct.json is not exist."
    else
        compare_md5 ${input_dir}/${step_path}/${rank_path}/construct.json ${baseline_dir}/${step_path}/${rank_path}/construct.json
        if [ $? -eq 1 ];then
            handle_error "${casename} construct.json check fail"
        fi
    fi

    # check stack.json
    if [ ! -f ${input_dir}/${step_path}/${rank_path}/stack.json ]; then
        handle_error "[ERROR] stack.json is not exist."
    else
        compare_lines ${input_dir}/${step_path}/${rank_path}/construct.json ${baseline_dir}/${step_path}/${rank_path}/construct.json
        if [ $? -eq 1 ];then
            handle_error "${casename} stack.json check fail"
        fi
    fi



}

# 比較大模型方法
validate_output_directory() {
    local json_path=$1
    local output_dir=$2
    local baseline_dir=$3

    # 提取 step 和 rank
    steps=$(jq -r '.step[]' "${json_path}")
    ranks=$(jq -r '.rank[]' "${json_path}")
    # 如果 steps 为空，则设置默认值为 0
    if [ -z "$steps" ]; then
        steps="0"
        echo "[WARNING] No steps found in ${json_path}. Defaulting to step=0."
    fi

    # 如果 ranks 为空，则设置默认值为 0
    if [ -z "$ranks" ]; then
        ranks="0"
        echo "[WARNING] No ranks found in ${json_path}. Defaulting to rank=0."
    fi

    for step in ${steps}; do
        for rank in ${ranks}; do
            check_dump_json "${output_dir}" "${baseline_dir}" "${rank}" "${step}"
            check_memory_and_time_variation "${output_dir}" "${baseline_dir}" "${rank}"
        done
    done


    echo "[Info] Output directory validation fit."
}



check_memory_and_time_variation() {
    local output_dir=$1
    local baseline_dir=$2
    local rank=$3

    # 定义阈值
    local COSINE_THRESHOLD=0.99
    local REL_ERROR_THRESHOLD=0.5

    # 读取 baseline 和 output 日志文件中的显存峰值和耗时
    local baseline_log="${baseline_dir}/pp_log/worker_${rank}.log"
    local output_log="${output_dir}/pp_log/worker_${rank}.log"

    echo "[INFO] Reading logs: Baseline log - ${baseline_log}, Output log - ${output_log}"
    # 提取 baseline 和 output 中的显存峰值和耗时
    local baseline_mem=$(grep -oP "显存峰值\s*[:：]?\s*\K[\d.]+(?=GB)" "${baseline_log}" | sort -nr | head -n 1)
    local output_mem=$(grep -oP "显存峰值\s*[:：]?\s*\K[\d.]+(?=GB)" "${output_log}" | sort -nr | head -n 1)

    local baseline_times=$(grep -oP "cost time:\s+\K[\d.]+" "${baseline_log}" | sort -nr)
    local output_times=$(grep -oP "cost time:\s+\K[\d.]+" "${output_log}" | sort -nr)

    echo "[INFO] Baseline memory peak: ${baseline_mem}GB, Output memory peak: ${output_mem}GB"


    local baseline_time_second=$(echo "$baseline_times" | sed -n '2p')
    local output_time_second=$(echo "$output_times" | sed -n '2p')

    echo "[INFO] Baseline second highest cost time: ${baseline_time_second}s, Output second highest cost time: ${output_time_second}s"

    # 检查 baseline_mem 是否为空
    if [ -z "$baseline_mem" ]; then
        echo "[WARNING] No memory peak found in baseline log for rank ${rank}. Skipping memory comparison."
    else
        # 比较显存峰值
        if (( $(echo "$output_mem > $baseline_mem * 1.1" | bc -l) )); then
            handle_error "[ERROR] Memory peak increased by more than 10%. Baseline: ${baseline_mem}GB, Output: ${output_mem}GB."
        else 
            echo "[INFO] Memory peak comparison fit for rank ${rank}."
        fi
    fi

    # 比较耗时
    if [ -z "$baseline_time_second" ]; then
        echo "[WARNING] No cost time found in baseline log for rank ${rank}. Skipping cost time comparison."
    else
        if (( $(echo "$output_time_second > $baseline_time_second * 1.2" | bc -l) )) && (( $(echo "$output_time_second - $baseline_time_second > 1.3" | bc -l) )); then
            handle_error "[ERROR] Cost time increased by more than 20% and time difference is greater than 1s. Baseline: ${baseline_time_second}s, Output: ${output_time_second}s."
        else
            echo "[INFO] Cost time comparison fit for rank ${rank}."
        fi
    fi
}



delete_dump_files() {
    local folder_name="dump_tensor_data"
    while true; do
        for step in {0..10}; do
            for rank in {0..7}; do
                dir_path="${testcase_result_dir}/step${step}/rank${rank}/${folder_name}"
                if [ -d "$dir_path" ]; then
                    echo "$dir_path deleted."
                    rm -rf $dir_path/*

                fi
            done
        done
    done
}
