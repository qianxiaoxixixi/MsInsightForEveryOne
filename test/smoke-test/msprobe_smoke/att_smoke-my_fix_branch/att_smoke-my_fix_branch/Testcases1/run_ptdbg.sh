#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run PT_Compare TestCase ----------------------------"
    for casename in $(ls ${testcases_dir}/pt_compare); do
        if [[ ${casename} =~ \.sh$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/pt_compare/${casename} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main