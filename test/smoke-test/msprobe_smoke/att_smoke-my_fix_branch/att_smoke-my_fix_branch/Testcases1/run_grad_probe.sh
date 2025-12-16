#!/bin/bash

testcases_dir=$(dirname $(readlink -f $0))
source ${testcases_dir}/../Utils/conf.sh
if [ ! -d $output_dir ]; then
    mkdir $output_dir
fi

output_dir="$output_dir/grad_probe"
testcases_dir=$(dirname $(readlink -f $0))

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run grad_probe TestCase ----------------------------"
    for casename in $(ls ${testcases_dir}/grad_probe); do
        if [[ ${casename} =~ \.sh$ ]]; then
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${casename} ----------------------------"
            echo "bash ${testcases_dir}/grad_probe/${casename} ${output_dir} ${pytorch_branchs} >> ${result_log}"
            bash ${testcases_dir}/grad_probe/${casename} ${output_dir} ${pytorch_branchs} >> ${result_log}
        fi
    done
    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}


main