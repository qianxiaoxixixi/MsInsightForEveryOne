import sys
import os

dump_path = sys.argv[1]
output = sys.argv[2]

from msprobe.mindspore.compare.distributed_compare import *


npu_dump_dir = '/home/maoyanlong/att_smoke_new/Testcases1/msa_dump_2/baseline/test_msa_qwen2.5_L0_statistics_none/step1'
bench_dump_dir = '/home/maoyanlong/att_smoke_new/Testcases1/msa_dump_2/baseline/test_msa_qwen2.5_L0_statistics_none/step0'

ms_compare_distributed(npu_dump_dir, bench_dump_dir, output_path=output)

