#!/bin/bash

# 假设你当前在 att_smoke 目录下
target_dir="../att_smoke_baseline"

# 创建目标目录
mkdir -p "${target_dir}"

# 查找当前目录中的所有 baseline 目录并进行移动
find . -type d -name "baseline" | while read -r baseline_path; do
    # 获取 baseline 目录的相对路径
    relative_path="${baseline_path#./}"

    # 创建目标目录结构
    target_path="${target_dir}/${relative_path}"
    mkdir -p "$(dirname "${target_path}")"

    # 移动 baseline 目录到目标目录
    mv "${baseline_path}" "${target_path}"
done
