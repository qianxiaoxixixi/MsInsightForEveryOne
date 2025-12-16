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
    cd ${cur_dir}/test_data/layer_mapping_model
    train1_log="train1.log"
    train2_log="train2.log"
    rm -f ${train1_log}
    rm -f ${train2_log}
    
    python train1.py  > ${train1_log} 2>&1 &
    python train2.py  > ${train2_log} 2>&1 &
    
    sleep 10

    TIMEOUT=120
    START_TIME=$(date +%s)

    while true; do
        if grep -q "msprobe ends successfully." ${cur_dir}/test_data/layer_mapping_model/${train1_log} && grep -q "msprobe ends successfully." ${cur_dir}/test_data/layer_mapping_model/${train2_log}; then
            echo "[info]: Find 'msprobe ends successfully.' in log."

            msprobe -f pytorch graph -i ${cur_dir}/test_data/layer_mapping_model/compare.json -o ${testcase_result_dir} -lm ${cur_dir}/test_data/layer_mapping_model/mapping.yaml >> ${testcase_result_dir}/plog.txt
            end_time=$(date "+%s")
            duration_time=$(( ${end_time} - ${start_time} ))
            grep "Exporting compare graph result successfully, the result file is saved in" ${testcase_result_dir}/plog.txt
            if [ $? -ne 0 ]; then
                echo "[error]: tool running abnormally."
                echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
                exit 1
            fi

            vis_file=$(find "${testcase_result_dir}" -maxdepth 1 -type f -name 'compare*.vis.db' -print -quit)
            if [ -z "$vis_file" ]; then
                echo "[error]: vis.db file does not exist."
                echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
                exit 1
            fi

            python3 ${cur_dir}/src/validate_visualization_layer_mapping_dump2compare_statistics.py ${vis_file} >> ${testcase_result_dir}/plog.txt
            grep "Validate the .vis.db file successfully." ${testcase_result_dir}/plog.txt
            if [ $? -ne 0 ]; then
                echo "[ERROR]: validate the .vis.db file fail."
                echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
                exit 1
            fi
            break
        fi

        CURRENT_TIME=$(date +%s)
        ELAPSED_TIME=$(( CURRENT_TIME - START_TIME ))

        if [ $ELAPSED_TIME -ge $TIMEOUT ]; then
            end_time=$(date "+%s")
            duration_time=$(( ${end_time} - ${start_time} ))
            echo "[error]: run resnet18 failed."
            echo "${file_name} fail ${duration_time} ${pytorch_branchs}"
            exit 1
            break
        fi

        sleep 5
    done


    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"

}

main

