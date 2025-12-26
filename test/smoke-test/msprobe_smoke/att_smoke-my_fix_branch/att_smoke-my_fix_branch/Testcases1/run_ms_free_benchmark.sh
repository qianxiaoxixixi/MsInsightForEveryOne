#!/bin/bash

ORI_MS_VERSION=`python3 -c 'import mindspore as ms; print(ms.__version__)'`
if [[ ${ORI_MS_VERSION} != "2.6.0" ]]; then
    pip3 uninstall mindspore -y
    pip3 install /data02/maoyanlong/about_smoke/soft_in_use/old_mindspore_large/mindspore-*.whl
fi


test_level=$1
if [ ! $test_level ]; then
    test_level=1
fi

cur_path=$(dirname $(readlink -f $0))
source ${cur_path}/../Utils/conf.sh
if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

testcases_dir=${cur_path}/ms_free_benchmark

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run MsFreeBenchmark TestCase ----------------------------"
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

if [[ ${ORI_MS_VERSION} != "2.6.0" ]]; then
    pip3 uninstall mindspore -y
    pip3 install /data02/maoyanlong/about_smoke/soft_in_use/common/mindspore-*.whl
fi

