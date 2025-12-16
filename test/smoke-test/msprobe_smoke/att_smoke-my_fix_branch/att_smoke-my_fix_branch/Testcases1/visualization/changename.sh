#!/bin/bash

# 参数检查
if [ $# -ne 2 ]; then
    echo "Usage: $0 <old_extension> <new_extension>"
    echo "Example: $0 bak sh"
    exit 1
fi

old_ext="$1"
new_ext="$2"

# 获取脚本的绝对路径（处理符号链接）
script_path=$(readlink -f "$0")

shopt -s dotglob
for file in *."$old_ext"; do
    # 获取当前文件的绝对路径
    file_path=$(readlink -f "$file")
    
    # 跳过脚本自身
    if [ "$file_path" = "$script_path" ]; then
        continue
    fi

    if [ -f "$file" ]; then
        base_name="${file%."$old_ext"}"
        
        # 检查是否需要保留扩展名
        if [[ "$base_name" == *."$new_ext" ]]; then
            new_name="$base_name"
        else
            new_name="${base_name}.$new_ext"
        fi
        
        # 执行重命名操作
        mv -- "$file" "$new_name"
    fi
done
shopt -u dotglob
