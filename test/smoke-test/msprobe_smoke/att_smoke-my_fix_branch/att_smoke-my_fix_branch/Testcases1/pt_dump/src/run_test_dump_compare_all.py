import sys
import os
from msprobe.pytorch import *


dump_path = sys.argv[1]
output = sys.argv[2]

npu_dump_dir = os.path.join(dump_path, 'step0')
bench_dump_dir = npu_dump_dir

compare_distributed(npu_dump_dir, bench_dump_dir, output)