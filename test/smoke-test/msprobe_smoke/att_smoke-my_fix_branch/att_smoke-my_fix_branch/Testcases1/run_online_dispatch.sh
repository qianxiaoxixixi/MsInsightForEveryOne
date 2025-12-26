#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

if [ $# -eq 1 ]; then
    level_to_run=$1
else
    level_to_run=4
fi

if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

online_dispatch_test_case_dir=${testcases_dir}/online_dispatch
smoke_level_mapping_file=${online_dispatch_test_case_dir}/smoke_level_mapping.txt

function get_value {
    local key=$1
    awk -F' ' -v k="$key" '$1 == k {print $2}' "$smoke_level_mapping_file"
}

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run online_dispatch TestCase ----------------------------"
    for casename in $(ls ${testcases_dir}/online_dispatch); do
        smoke_level=$(get_value ${casename})
        if [[ ${casename} =~ \.sh$ ]] && [ $level_to_run -ge $smoke_level ]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/online_dispatch/${casename} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main