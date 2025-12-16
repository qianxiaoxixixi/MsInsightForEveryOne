
import sys
import torch 
from msprobe.pytorch.online_dispatch.dump_compare import dispatch_workflow, DispatchRunParam, DisPatchDataInfo
from msprobe.pytorch.online_dispatch.compare import Comparator


dump_path = sys.argv[1]

def compare(dump_path):
    result_csv_path = dump_path + '/result.csv'
    detail_csv_path = dump_path + '/detail.csv'

    comparator = Comparator(result_csv_path, detail_csv_path, False)
    run_param = DispatchRunParam(debug_flag = False, device_id = 0, root_npu_path= dump_path, root_cpu_path=dump_path, process_num=0, comparator=comparator)
    run_param.aten_api = "addmm"
    run_param.aten_api_overload_name = "default"
    run_param.single_api_index = 1
    run_param.dump_flag = True
    run_param.api_index = 2

    cpu_args=1
    func = torch.ops.aten.addmm.default
    npu_out_cpu_1 = torch.tensor([1,2],dtype=torch.int32)
    npu_out_cpu_2 = torch.tensor([3,4],dtype=torch.int32)
    cpu_out = torch.tensor([1,2],dtype=torch.int32)

    data_info_1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_1, cpu_out, None)
    # data_info_1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_1, cpu_out)
    dispatch_workflow(run_param, data_info_1)
    data_info_2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_2, cpu_out, None)
    # data_info_2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_2, cpu_out)
    dispatch_workflow(run_param, data_info_2)

if __name__ =='__main__':
    
    compare(dump_path)