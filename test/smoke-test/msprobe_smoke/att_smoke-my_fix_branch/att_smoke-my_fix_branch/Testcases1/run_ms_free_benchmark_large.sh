#!/bin/bash

cur_path=$(dirname $(readlink -f $0))
source ${cur_path}/../Utils/conf.sh
if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

testcases_dir=${cur_path}/ms_free_benchmark

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run MsFreeBenchmark Large TestCase ----------------------------"
    valid_suffix="L2.sh"
    for casename in $(ls ${testcases_dir}); do
        if [[ ${casename} =~ $valid_suffix$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/${casename} >> ${result_log}
        fi
    done
    valid_suffix="L4.sh"
    for casename in $(ls ${testcases_dir}); do
        if [[ ${casename} =~ $valid_suffix$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/${casename} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main