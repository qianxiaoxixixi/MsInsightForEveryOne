#!/bin/bash
##################### 所有脚本一样 ###################
cur_dir=$(dirname $(readlink -f $0))



source ${cur_dir}/../../Utils/conf.sh

main() {
    file_name=$(basename $0 .sh)
    testcase_result_dir="${output_dir}/visualization/${file_name}"
    if [ ! -d ${testcase_result_dir} ]; then
        mkdir -p ${testcase_result_dir}
    else
        rm -rf ${testcase_result_dir}/*
    fi

    start_time=$(date "+%s")
    cd ${cur_dir}/test_data/
    msprobe -f pytorch graph -i ${cur_dir}/test_data/build_dist_p2p.json -o ${testcase_result_dir} >> ${testcase_result_dir}/plog.txt
    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))

    grep "Successfully exported build graph results" ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[error]: tool running abnormally."
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    vis_files=$(find "${testcase_result_dir}" -maxdepth 1 -type f -name 'build*.vis.db')
    vis_count=$(echo "$vis_files" | grep -c '^')

    if [ "$vis_count" -ne 1 ]; then
        echo "[error]: The number of vis.db files is not 1"
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    vis_file=$(find "${testcase_result_dir}" -maxdepth 1 -type f -name 'build*.vis.db' -print -quit)

    python3 ${cur_dir}/src/validate_visualization_build_graph_dist_p2p.py ${vis_file} >> ${testcase_result_dir}/plog.txt

    grep "Validate the .vis.db file successfully." ${testcase_result_dir}/plog.txt
    if [ $? -ne 0 ]; then
        echo "[ERROR]: validate the .vis.db file fail."
        echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
        exit 1
    fi

    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"
}

main
