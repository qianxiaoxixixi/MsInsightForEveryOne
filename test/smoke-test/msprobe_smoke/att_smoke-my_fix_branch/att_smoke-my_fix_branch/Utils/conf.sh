#!/bin/bash

pytorch_branchs=pytorch21
conda_env_name=pytorch21_version
base_dir=/home/maoyanlong/att_smoke_new
testcases=${base_dir}/Testcases1
workspace=${base_dir}/WorkSpace1

download_path=${workspace}/pkg
output_dir=${workspace}/output
result_log=${workspace}/output/plog.txt
result_log2=${workspace}/output2/plog.txt
smoke_log="${workspace}/smoke_$(date "+%Y%m%d%H%M%S").log"
after_move_smoke_log="/data00/maoyanlong/WorkSpace1/smoke_$(date "+%Y%m%d%H%M%S").log"

# for api_accuracy_checking
att_compare_path=${workspace}/pkg/debug/accuracy_tools/msprobe/pytorch/api_accuracy_checker/compare
att_ut_path=${workspace}/pkg/debug/accuracy_tools/msprobe/pytorch/api_accuracy_checker/run_ut
# run_ut_config=${workspace}/pkg/debug/accuracy_tools/msprobe/pytorch/api_accuracy_checker/config.yaml
run_ut_config=/home/maoyanlong/miniconda3/envs/pytorch21_version/lib/python3.8/site-packages/msprobe/pytorch/api_accuracy_checker/config.yaml
