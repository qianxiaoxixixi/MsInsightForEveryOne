#!/bin/echo Warning:this is a library should be sourced!


recover_atat_daily() {
    # recover conda env atat for daily smoke
    src=/home/maoyanlong/att_smoke_new/WorkSpace1/pkg/debug/accuracy_tools/msprobe
    # yc
    # dst=/home/maoyanlong/miniconda3/envs/smoking_py39_pt27/lib/python3.8/site-packages/msprobe
    dst=/home/maoyanlong/miniconda3/envs/smoking_py39_pt27/lib/python3.9/site-packages/msprobe
    rm -rf ${dst}
    cp -r ${src} ${dst}
}

## 加一個setup函數編包安裝
compile_pkg() {
    cur_path=$(pwd)
    cd ${download_path}/debug/accuracy_tools
    ARCH=$(uname -m)
    if [[ ! -e ./build_dependency && -e "/home/maoyanlong/att_smoke_new/mstt_env/3rd_party_dependency_${ARCH}/build_dependency" ]]; then
        ln -s "/home/maoyanlong/att_smoke_new/mstt_env/3rd_party_dependency_${ARCH}/build_dependency" ./build_dependency
    fi
    if [[ ! -e ./third_party && -e "/home/maoyanlong/att_smoke_new/mstt_env/3rd_party_dependency_${ARCH}/third_party" ]]; then
        ln -s "/home/maoyanlong/att_smoke_new/mstt_env/3rd_party_dependency_${ARCH}/third_party" ./third_party
    fi
    python setup.py bdist_wheel --include-mod=adump
    cd /
    pip uninstall mindstudio-probe -y
    pip cache purge
    pip install ${download_path}/debug/accuracy_tools/dist/*.whl
    cd ${cur_path}
}

init_env_daily() {
    # init env
    source /home/maoyanlong/.bashrc
    conda init
    conda activate smoking_py39_pt27
    # source /home/maoyanlong/att_smoke_env/ascend-toolkit/set_env.sh
    # source /home/maoyanlong/att_smoke_env/ascend-toolkit/set_env.sh
    #export PYTHONPATH=${download_path}/debug/accuracy_tools:$PYTHONPATH
}

init_env_moe() {
    # init env
    source /home/maoyanlong/.bashrc
    conda init
    conda activate new_smoking_py39_pt25_lcw
    # source /home/maoyanlong/att_smoke_env/ascend-toolkit/set_env.sh
    # source /home/maoyanlong/att_smoke_env/ascend-toolkit/set_env.sh
    # export PYTHONPATH=${download_path}/debug/accuracy_tools:$PYTHONPATH
}


mindspore_init_env_daily() {
    # init env
    source /home/maoyanlong/.bashrc
    conda init
    conda activate smoking_py39_pt27
    # conda activate pytorch21_copy
    # source /home/maoyanlong/att_smoke_env/ascend-toolkit/set_env.sh
    # source /home/maoyanlong/att_smoke_env/ascend-toolkit/set_env.sh
    # export PYTHONPATH=${download_path}/debug/accuracy_tools:$PYTHONPATH
}

filter_and_print() {
    success_info=()
    failed_info=()
    while read line; do
        # if [[ $line =~ "fail" ]]; then
        if [[ $line =~ fail[[:space:]]+[0-9]+ ]]; then
            failed_info+=("${line}")
        # elif [[ $line =~ "pass" ]]; then
        elif [[ $line =~ pass[[:space:]]+[0-9]+ ]]; then
            success_info+=("${line}")
        fi
    done < $1
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    for success_entry in "${success_info[@]}"; do
        echo $success_entry
    done
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    for failed_entry in "${failed_info[@]}"; do
        echo $failed_entry
    done
    echo "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
    if [ -n "$failed_info" ]; then
        echo "att smoke not pass. Please locate the reason."
        exit 1
    fi
}

zip_and_print() {
    local output=$1
    time_info=$(date "+%Y%m%d-%H%M%S")
    local zip_output="$(dirname ${output})/output_${time_info}.tar.gz"
    local move_zip_output="/data00/maoyanlong/WorkSpace1/output_${time_info}.tar.gz"
    tar -cf ${zip_output} ${output}
    echo "zip_output: ${zip_output}"
    echo "after_move_zip_output: ${move_zip_output}"

}

# 函数：根据是否传入密码执行 sudo pkill -9 python
function pkill_python() {
    local password="${1:-}"  # 如果未提供参数，则 password 为空

    if [ -n "$password" ]; then
        echo "$password" | sudo -S pkill -9 python
        echo "$password" | sudo -S pkill -9 msprobe
        echo "pkill executed with provided password."
    else
        echo "No password provided, skipping pkill."
    fi
}
