#!/bin/bash

# 假设你当前在 att_smoke 目录下
source_dir="../att_smoke_baseline"

# 查找目标目录中的所有 baseline 目录并进行移动
find "${source_dir}" -type d -name "baseline" | while read -r baseline_path; do
    # 获取 baseline 目录的相对路径
    relative_path="${baseline_path#${source_dir}/}"

    # 目标位置
    target_path="./${relative_path}"

    # 创建原始目录结构
    mkdir -p "$(dirname "${target_path}")"

    # 移动 baseline 目录回原来的位置
    mv "${baseline_path}" "${target_path}"
done
