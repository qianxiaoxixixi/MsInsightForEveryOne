#!/bin/bash

export HCCL_BUFFSIZE=1
export HCCL_EXEC_TIMEOUT=600
export HCCL_DETERMINISTIC=true
export ASCEND_LAUNCH_BLOCKING=1

# run pp process
export ASCEND_RT_VISIBLE_DEVICES='0,1,2,3'
log_path=/home/maoyanlong/att_smoke_new/WorkSpace1/output/ms_free_benchmark/test_ms_fb_nor_check_no_L4/pp_log
IFS="," read -r -a devices <<< "$ASCEND_RT_VISIBLE_DEVICES"
work_num=${#devices[@]}
msrun --worker_num "$work_num" --local_worker_num="$work_num" --master_port=8130 --log_dir="$log_path" --join=True --cluster_time_out=300 pp_train.py --run_mode pp
