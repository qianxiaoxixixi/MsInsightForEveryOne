#!/bin/bash

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
    valid_suffix="_test_L$i.sh"
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