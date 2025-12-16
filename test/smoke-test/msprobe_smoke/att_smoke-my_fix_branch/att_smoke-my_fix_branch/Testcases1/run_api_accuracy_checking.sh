#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh

if [ $# -eq 1 ]; then
    level_to_run=$1
else
    level_to_run=2
fi

if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

api_accuracy_checker_test_case_dir=${testcases_dir}/api_accuracy_checking
smoke_level_mapping_file=${api_accuracy_checker_test_case_dir}/smoke_level_mapping.txt

function get_value {
    local key=$1
    awk -F' ' -v k="$key" '$1 == k {print $2}' "$smoke_level_mapping_file"
}

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run api_accuracy_checking TestCase ----------------------------"
    echo "[Info][$(date "+%F %T")] ---------------------------- level to run is ${level_to_run} -------------------------------------"
    for casename in $(ls ${testcases_dir}/api_accuracy_checking); do
        smoke_level=$(get_value ${casename})
        if [[ ${casename} =~ \.sh$ ]] && [ $level_to_run -ge $smoke_level ]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/api_accuracy_checking/${casename} 2>&1 | grep -v "tools is dumping api:" >> ${result_log}
            # bash ${testcases_dir}/api_accuracy_checking/${casename} ${output_dir} ${pytorch_branchs} 2>&1 | grep -v "tools is dumping api:" >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main
