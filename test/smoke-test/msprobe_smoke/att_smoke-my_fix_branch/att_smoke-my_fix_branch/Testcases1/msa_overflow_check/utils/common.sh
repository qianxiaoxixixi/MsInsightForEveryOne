
handle_error() {
    start_time=${start_time:=0}
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    # duration_time=${duration_time:=0}
#
    echo "${file_name} FAIL: $1"
    echo "${file_name} fail ${duration_time} mindspore"
    exit 1
#
}

# 比較大模型方法
validate_output_directory_first() {
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


    for rank in ${ranks}; do
        # check_dump_json "${output_dir}" "${baseline_dir}" "${rank}" "${step}"
        check_memory_and_time_variation_first "${output_dir}" "${baseline_dir}" "${rank}"
    done

    echo "[Info] Output directory validation passed."
}

validate_output_directory_second() {
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


    for rank in ${ranks}; do
        # check_dump_json "${output_dir}" "${baseline_dir}" "${rank}" "${step}"
        check_memory_and_time_variation_second "${output_dir}" "${baseline_dir}" "${rank}"
    done

    echo "[Info] Output directory validation passed."
}

check_memory_and_time_variation_first() {
    local output_dir=$1
    local baseline_dir=$2
    local rank=$3


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


    local baseline_time_second=$(echo "$baseline_times" | sed -n '1p')
    local output_time_second=$(echo "$output_times" | sed -n '1p')

    echo "[INFO] Baseline second highest cost time: ${baseline_time_second}s, Output second highest cost time: ${output_time_second}s"

    # 检查 baseline_mem 是否为空
    if [ -z "$baseline_mem" ]; then
        echo "[WARNING] No memory peak found in baseline log for rank ${rank}. Skipping memory comparison."
    else
        # 比较显存峰值
        if (( $(echo "$output_mem > $baseline_mem * 1.1" | bc -l) )); then
            handle_error "[ERROR] Memory peak increased by more than 10%. Baseline: ${baseline_mem}GB, Output: ${output_mem}GB."
        else
            echo "[INFO] Memory peak comparison passed for rank ${rank}."
        fi
    fi

    # 比较耗时
    if [ -z "$baseline_time_second" ]; then
        echo "[WARNING] No cost time found in baseline log for rank ${rank}. Skipping cost time comparison."
    else
        if (( $(echo "$output_time_second > $baseline_time_second * 1.2" | bc -l) )); then
            handle_error "[ERROR] Cost time increased by more than 20%. Baseline: ${baseline_time_second}s, Output: ${output_time_second}s."
        else
            echo "[INFO] Cost time comparison passed for rank ${rank}."
        fi
    fi
}

check_memory_and_time_variation_second() {
    local output_dir=$1
    local baseline_dir=$2
    local rank=$3


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
            echo "[INFO] Memory peak comparison passed for rank ${rank}."
        fi
    fi

    # 比较耗时
    if [ -z "$baseline_time_second" ]; then
        echo "[WARNING] No cost time found in baseline log for rank ${rank}. Skipping cost time comparison."
    else
        if (( $(echo "$output_time_second > $baseline_time_second * 1.2" | bc -l) )); then
            handle_error "[ERROR] Cost time increased by more than 20%. Baseline: ${baseline_time_second}s, Output: ${output_time_second}s."
        else
            echo "[INFO] Cost time comparison passed for rank ${rank}."
        fi
    fi
}
