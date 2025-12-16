from msprobe.mindspore.compare.distributed_compare import *


npu_dump_dir = '/home/maoyanlong/att_smoke_new/Testcases1/msprobe_ms_dump/baseline/mindtorch_Larger_L1_task_md5_step_all_rank_all_scop_all_list_all_data_mode_all/step2'
bench_dump_dir = '/home/maoyanlong/att_smoke_new/Testcases1/msprobe_ms_dump/baseline/mindtorch_Larger_L1_task_md5_step_all_rank_all_scop_all_list_all_data_mode_all/step2'

ms_compare_distributed(npu_dump_dir, bench_dump_dir, output_path='./output')
