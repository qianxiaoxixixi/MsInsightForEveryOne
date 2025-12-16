#!/bin/bash

# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/home/maoyanlong/miniconda3/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/home/maoyanlong/miniconda3/etc/profile.d/conda.sh" ]; then
        . "/home/maoyanlong/miniconda3/etc/profile.d/conda.sh"
    else
        export PATH="/home/maoyanlong/miniconda3/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<

test_level=$1
if [ ! $test_level ]; then
    test_level=4
fi

cur_path=$(dirname $(readlink -f $0))
source ${cur_path}/../Utils/conf.sh
if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

testcases_dir=${cur_path}/msa_overflow_check

ORI_PYTHON=$(echo "$PYTHONPATH")
ORI_LIB_PATH=$(echo "$LD_LIBRARY_PATH")

cd /data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM
source set_env.sh
pip3 uninstall mindstudio-probe -y
if [ ! -d "./mstt/.git" ]; then
    rm -rf mstt/
    git clone https://gitcode.com/Ascend/mstt.git
else
    cd mstt
    git checkout master
    git reset --hard
    git pull
fi

export PYTHONPATH=/data02/maoyanlong/Model/msadater/Qwen2.5/MindSpeed-Core-MS/MindSpeed-LLM/mstt/debug/accuracy_tools:$PYTHONPATH
# export PYTHONPATH=/home/maoyanlong/att_smoke_new/Testcases1/ms_overflow_check/gitcode/mstt/debug/accuracy_tools::$PYTHONPATH

cd /home/maoyanlong/att_smoke_new 

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run MSA OverflowCheck TestCase ----------------------------"
    for((i=1;i<=test_level;i++));  
    do
    valid_suffix="L$i.sh"
        for casename in $(ls ${testcases_dir}); do
            if [[ ${casename} =~ $valid_suffix$ ]]; then
                echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
                bash ${testcases_dir}/${casename} >> ${result_log}
            fi
        done
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main

conda deactivate
export PYTHONPATH=$ORI_PYTHON
export LD_LIBRARY_PATH=$ORI_LIB_PATH
