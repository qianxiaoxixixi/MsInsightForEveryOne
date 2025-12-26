#!/bin/bash

test_level=$1
if [ ! $test_level ]; then
    test_level=1
fi

cur_path=$(dirname $(readlink -f $0))
source ${cur_path}/../Utils/conf.sh

testcases_dir=${cur_path}/ms_acl_dump

main() {
    echo "[Info][$(date "+%F %T")] ---------------------------- Begin Run MsAclDump TestCase ----------------------------"

    single_rank_testcases=()
    single_rank_testcase_index=0
    multi_rank_testcases=()
    multi_rank_testcase_index=0
    for((i=1;i<=test_level;i++));
    do
        valid_suffix="L$i.sh"
        for casename in $(ls ${testcases_dir}); do
            if [[ ${casename} =~ $valid_suffix$ ]]; then
                if [ $i == 1 ] || [ $i == 3 ]; then
                    single_rank_testcases[single_rank_testcase_index]=${casename}
                    single_rank_testcase_index=`expr $single_rank_testcase_index + 1`
                fi
                if [ $i == 2 ] || [ $i == 4 ]; then
                    multi_rank_testcases[multi_rank_testcase_index]=${casename}
                    multi_rank_testcase_index=`expr $multi_rank_testcase_index + 1`
                fi
            fi
        done
    done

    index=0
    while (($index < $single_rank_testcase_index))
    do
        device_ids=$(/usr/local/bin/npu-smi info | grep No | awk '{print $8}')
        device_array=($(echo $device_ids))
        actual_count=${#device_array[@]}
        next_index=`expr $index + $actual_count`

        if (( $next_index > $single_rank_testcase_index )); then
            actual_count=`expr $actual_count + $single_rank_testcase_index - $next_index`
        fi
        for((i=0;i<${actual_count};i++))
        do
            echo "[Info][$(date "+%F %T")] ---------------------------- Execution ${single_rank_testcases[index]} ----------------------------"
            bash ${testcases_dir}/${single_rank_testcases[index]} ${device_array[i]}  >> ${result_log} &
            index=`expr $index + 1`
        done

        sleep 20
    done
    wait

    echo "[Info][$(date "+%F %T")] ---------------------------- Testcases execution completed ----------------------------"
}

main
