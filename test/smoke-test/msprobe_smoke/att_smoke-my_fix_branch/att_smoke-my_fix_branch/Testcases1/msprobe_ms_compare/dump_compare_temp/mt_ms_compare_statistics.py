from msprobe.mindspore.compare.distributed_compare import *


npu_dump_dir = '/home/maoyanlong/att_smoke_new/Testcases1/msprobe_ms_dump/baseline/mindtorch_Larger_L1_task_statistics_step_all_rank_0_1_scope_all_list_all_data_mode_all/step1'
bench_dump_dir = '/home/maoyanlong/att_smoke_new/Testcases1/msprobe_ms_dump/baseline/mindtorch_Larger_L1_task_statistics_step_all_rank_0_1_scope_all_list_all_data_mode_all/step0'

ms_compare_distributed(npu_dump_dir, bench_dump_dir, output_path='./output')

