#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run msprobe_ms_compare TestCase ----------------------------"
    for casename in $(ls ${testcases_dir}/msprobe_ms_compare/sh); do
        if [[ ${casename} =~ \.sh$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/msprobe_ms_compare/sh/${casename} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main