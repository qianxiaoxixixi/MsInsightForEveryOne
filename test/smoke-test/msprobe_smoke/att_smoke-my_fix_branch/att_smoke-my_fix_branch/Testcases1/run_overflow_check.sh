#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run Overflow checking TestCase ----------------------------"
    for casename in $(ls ${testcases_dir}/overflow_check); do
        if [[ ${casename} =~ \.sh$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
#            bash ${testcases_dir}/overflow_check/${casename} ${output_dir} ${pytorch_branchs} >> ${result_log}
            bash ${testcases_dir}/overflow_check/${casename} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}


main