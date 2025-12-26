#!/bin/bash
 
testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh
 
cd ${download_path}
cd ${testcases_dir}
 
export PYTHONPATH=$PYTHONPATH:${download_path}/debug/accuracy_tools/
 
 
if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi
 
main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run nan_analyze TestCase ----------------------------------"
    for casename in $(ls ${testcases_dir}/nan_analyze); do
        if [[ ${casename} =~ \.sh$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            bash ${testcases_dir}/nan_analyze/${casename} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}
 
main
 
cd ${download_path}
