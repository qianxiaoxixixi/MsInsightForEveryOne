#!/bin/bash

# Requires one parameters: error information
handle_error() {
    duration_time=${duration_time:=0}
    echo "${test_name} fail: $1"
    echo "${test_name} fail ${duration_time} ${pytorch_branchs}"
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

compare_dir_empty() { #off模式下npu和cpu文件夹为空
    input_dir=$1

    if [ ! -d "${input_dir}/cpu" ]; then
        handle_error "[ERROR] cpu dir is not exist."
    else
        count=$(find "${input_dir}/cpu" -mindepth 1 | wc -l)
        if [ "$count" -gt 0 ]; then
            handle_error "[ERROR] cpu dir should be empty when OFF mode."
        fi
    fi

    if [ ! -d "${input_dir}/npu" ]; then
        handle_error "[ERROR] npu dir is not exist."
    else
        count=$(find "${input_dir}/npu" -mindepth 1 | wc -l)
        if [ "$count" -gt 0 ]; then
            handle_error "[ERROR] npu dir should be empty when OFF mode."
        fi
    fi
    
}

compare_dir_equal() {
    input_dir=$1
    gt_dir=$2

    files1_cpu=($(ls -A "${input_dir}/cpu" 2>/dev/null))  
    files1_npu=($(ls -A "${input_dir}/npu" 2>/dev/null)) 
    files2_cpu=($(ls -A "${gt_dir}/cpu" 2>/dev/null)) 
    files2_npu=($(ls -A "${gt_dir}/npu" 2>/dev/null))  

    # 检查文件数量是否相同  
    if [ ${#files1_cpu[@]} -ne ${#files2_cpu[@]} ]; then  
        echo "cpu文件数量不同。"
        handle_error "[ERROR] cpu文件数量不同."   
    fi  
    if [ ${#files1_npu[@]} -ne ${#files2_npu[@]} ]; then  
        echo "npu文件数量不同。"   
        handle_error "[ERROR] npu文件数量不同。." 
        
    fi  
    
    # 排序文件列表以便于比较  
    IFS=$'\n' sorted_files1_cpu=($(sort <<<"${files1_cpu[*]}"))  
    IFS=$'\n' sorted_files2_cpu=($(sort <<<"${files2_cpu[*]}"))  
    unset IFS  

    IFS=$'\n' sorted_files1_npu=($(sort <<<"${files1_npu[*]}"))  
    IFS=$'\n' sorted_files2_npu=($(sort <<<"${files2_npu[*]}"))  
    unset IFS  
    
    # 检查文件名是否相同  
    for (( i=0; i<${#sorted_files1_cpu[@]}; i++ )); do  
        if [ "${sorted_files1_cpu[$i]}" != "${sorted_files2_cpu[$i]}" ]; then  
            echo "文件名不一致：${sorted_files1[$i]} 在 $dir1 中有，但在 $dir2 中找不到对应的文件。"  
            handle_error "文件名不一致：${sorted_files1[$i]} 在 $dir1 中有，但在 $dir2 中找不到对应的文件。" 
            exit 1  
        fi  
    done  
    for (( i=0; i<${#sorted_files1_npu[@]}; i++ )); do  
        if [ "${sorted_files1_npu[$i]}" != "${sorted_files2_npu[$i]}" ]; then  
            echo "文件名不一致：${sorted_files1[$i]} 在 $dir1 中有，但在 $dir2 中找不到对应的文件。"
            handle_error "文件名不一致：${sorted_files1[$i]} 在 $dir1 中有，但在 $dir2 中找不到对应的文件。"
            
            exit 1  
        fi  
    done  
}

compare_csv_file() {
    input_dir=$1
    gt_dir=$2

    result_csv=$(find $input_dir -type f -name "*result*" | head -n 1)
    detail_csv=$(find $input_dir -type f -name "*detail*" | head -n 1)
    gt_result_csv="${gt_dir}/result.csv"
    gt_detail_csv="${gt_dir}/detail.csv"

    # 检查输出件是否存在  
    if [ ! -f "$result_csv" ] || [ ! -f "$detail_csv" ]; then  
        handle_error "${result_csv} [ERROR] csv files do not exist."    
    fi  

    # md5校验
    compare_md5 ${result_csv} ${gt_result_csv}
    if [ $? -eq 1 ];then
        handle_error "result csv files md5 check fail"
    fi
    compare_md5 ${detail_csv} ${gt_detail_csv}
    if [ $? -eq 1 ];then
        handle_error "detail csv files md5 check fail"
    fi
}

compare_dump_tag() {
    input_dir=$1
    dir_tag_name="${input_dir##*/}"
    if [[ "$dir_tag_name" != *"tag"* ]]; then  
        handle_error "output dis not contains tag keyword"  
    fi  
}

compare_debug_log() {
    log_file=$1
    if ! grep -q "Dispatch Info" $log_file; then  
        handle_error "文件不包含关键字"  
    fi
}





