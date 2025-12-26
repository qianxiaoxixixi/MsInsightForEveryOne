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
    msprobe_dump_path="resnet18_statistics_mix"
    msprobe_dump_log="msprobe_dump_log_statistics.log"
    if [ -e ${msprobe_dump_path} ]; then
        rm -rf ${msprobe_dump_path}
    fi
    python resnet18.py -mt statistics -mdp ${msprobe_dump_path} --gpu 1 --epochs 1 --dummy > ${msprobe_dump_log} 2>&1 &

    TIMEOUT=120
    START_TIME=$(date +%s)

    while true; do
        if grep -q "msprobe ends successfully." ${msprobe_dump_log}; then
            echo "[info]: Find 'msprobe ends successfully.' in log."
            msprobe -f pytorch graph -i ${cur_dir}/test_data/resnet18_statistics_mix.json -o ${testcase_result_dir} >> ${testcase_result_dir}/plog.txt
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

            python3 ${cur_dir}/src/validate_visualization_resnet18_dump2compare_statistics.sh.py ${vis_file} >> ${testcase_result_dir}/plog.txt
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

    rm -rf ${msprobe_dump_path}
    rm -f ${msprobe_dump_log}

    end_time=$(date "+%s")
    duration_time=$(( ${end_time} - ${start_time} ))
    echo "${file_name} pass ${duration_time} ${pytorch_branchs}"

}

main

