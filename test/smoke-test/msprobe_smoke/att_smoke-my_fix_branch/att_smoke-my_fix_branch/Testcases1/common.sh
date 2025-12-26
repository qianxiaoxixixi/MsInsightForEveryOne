#!/bin/bash

# Requires one parameters: error information
handle_error() {
    duration_time=${duration_time:=0}
    echo "${file_name} fail: $1"
    echo "${file_name} fail ${duration_time}s ${pytorch_branchs}"
    exit 1
}

compare_size() {
    file1=$1
    file2=$2

    file1_size=$(stat -c %s $file1 | awk '{ printf "%.1f\n", $1/1024 }')
    file2_size=$(stat -c %s $file2 | awk '{ printf "%.1f\n", $1/1024 }')

    if [[ $file1_size != $file2_size ]]; then
        return 1
    fi
    return 0
}

compare_similarity() {
    file1=$1
    file2=$2

    lines_file1=$(wc -l < "$file1")  
    lines_file2=$(wc -l < "$file2")  

    # 计算行数的绝对差异  
    if [ "$lines_file1" -gt "$lines_file2" ]; then  
        line_diff=$((lines_file1 - lines_file2))
    else  
        line_diff=$((lines_file2 - lines_file1))
    fi
    percent_diff=$(echo "scale=2; $line_diff / $lines_file1 * 100" | bc)
    if (( $(echo "$percent_diff >= 20" | bc -l) )); then
        return 1
    fi
    return 0
}

compare_accuracy() {
    file1=$1
    file2=$2
    while IFS= read -r line1 && IFS= read -r line2 <&3; do
    if [ "$line1" != "$line2" ]; then
        if (echo "$line1" | grep -q -E '"md5":') && (echo "$line2" | grep -q -E '"md5":'); then
            continue
        elif (echo "$line1" | grep -q -E '"md5":') || (echo "$line2" | grep -q -E '"md5":'); then
            return 1
        fi

        if ! (echo "$line1" | grep -q -E '"Max":| "Min":| "Mean":| "Norm":| "value":| "Max_except_inf_nan":| "Min_except_inf_nan":') || \
            ! (echo "$line2" | grep -q -E '"Max":| "Min":| "Mean":| "Norm":| "value":| "Max_except_inf_nan":| "Min_except_inf_nan":'); then
            return 1
        fi
        str1=$(echo "$line1" | awk -F'[:,]' '{print $2}')
        str2=$(echo "$line2" | awk -F'[:,]' '{print $2}')

        path=$(dirname $(readlink -f "$0"))
        result=$(python3 "$path/utils/compare_accuracy.py" $str1 $str2)
        return $result
    fi
    done <"$file1" 3<"$file2"
    return 0
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

compare_dump() {
    input_dir=$1
    baseline_dir=$2

    input_count=$(find "$input_dir" -type f | wc -l)  
    baseline_count=$(find "$baseline_dir" -type f | wc -l)  

    # 比较两个目录的文件总数是否相同
    if [ "$input_count" -ne "$baseline_count" ]; then  
        handle_error "[ERROR] The number of files in input_dir is different from that in baseline_dir."  
    fi

    # 比较两个目录的文件内容是否相同  
    find "$baseline_dir" -type f | while read -r file1; do  
        # 计算相对路径
        relative_path="${file1#$baseline_dir/}"  
        file2="$input_dir/$relative_path"  

        # 检查对应的文件是否存在且相同
        if [[ -f "$file2" ]]; then
            # plog.txt文件直接跳过，即日志文件不比较，因为存在时间戳等信息。
            if [[ "$(basename "$file1")" == "plog.txt" ]]; then
                continue
            fi

            # pt文件比较，判断文件大小是否相同
            if [[ "$(basename "$file1")" == *.pt ]]; then
                compare_size $file1 $file2
                if [ $? -eq 1 ];then
                    handle_error "Failed to check the $file1 and $file2 files."
                fi
                continue
            fi

            # stack.json文件比较，判断文件行数是否有明显差异
            if [[ "$(basename "$file1")" == "stack.json" ]]; then
                compare_similarity $file1 $file2
                if [ $? -eq 1 ];then
                    handle_error "Failed to check the $file1 and $file2 files."
                fi
                continue  
            fi

            # dump.json文件比较，判断数据精度是否有明显差异
            if [[ "$(basename "$file1")" == "dump.json" ]]; then
                compare_accuracy $file1 $file2
                if [ $? -eq 1 ];then
                    handle_error "Failed to check the $file1 and $file2 files."
                fi
                continue
            fi

            # 比较其他文件内容
            compare_md5 $file1 $file2
            if [ $? -eq 1 ];then
                handle_error "Failed to check the $file1 and $file2 files."
            fi
        else
            handle_error "[ERROR] The $relative_path file does not exist in input_dir."  
        fi
    done

    # 返回0表示比对成功
    return 0
}
