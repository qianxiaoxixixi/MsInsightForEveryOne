#!/bin/bash

# 定义 output_dir 变量
output_dir=""  # 请根据实际情况修改此路径

# 检查 output_dir 是否设置、长度大于2且路径存在
if [[ -n "$output_dir" && ${#output_dir} -gt 2 && -e "$output_dir" ]]; then
    echo "准备删除目录: $output_dir"
    #rm -rf "$output_dir"
    if [[ $? -eq 0 ]]; then
        echo "成功删除目录: $output_dir"
    else
        echo "删除目录失败: $output_dir" >&2
        exit 1
    fi
else
    echo "条件不满足，未删除目录: $output_dir"
fi

