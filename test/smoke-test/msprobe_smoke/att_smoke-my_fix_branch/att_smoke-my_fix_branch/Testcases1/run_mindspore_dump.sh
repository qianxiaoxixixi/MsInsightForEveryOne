#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))

output=/home/maoyanlong/att_smoke_new/WorkSpace1/output
result_log=${output}/plog.txt

if [ ! -d $output ]; then
    mkdir $output
fi

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run mindspore_dump TestCase ----------------------------"
    for casename in $(ls ${testcases_dir}/mindspore_dump); do
        if [[ ${casename} =~ \.sh$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/mindspore_dump/${casename} 2>&1 | grep -v "tools is dumping api:" >> ${result_log}
            # bash ${testcases_dir}/mindspore_run_dump/${casename} ${output_dir} ${pytorch_branchs} 2>&1 | grep -v "tools is dumping api:" >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main
