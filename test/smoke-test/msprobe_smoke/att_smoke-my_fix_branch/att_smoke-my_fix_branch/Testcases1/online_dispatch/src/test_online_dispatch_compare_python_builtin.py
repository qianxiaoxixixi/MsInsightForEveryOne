
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
    npu_out_cpu_bool1 = True
    npu_out_cpu_bool2 = False
    cpu_out_bool1 = True
    npu_out_cpu_int1 = 2
    npu_out_cpu_int2 = 3
    cpu_out_int1 = 2
    npu_out_cpu_float1 = 2.2
    npu_out_cpu_float2 = 3.5
    cpu_out_float1 = 2.2
    npu_out_cpu_str1 = "compare"
    npu_out_cpu_str2 = "fa"
    cpu_out_str1 = "compare"
    

    data_info_bool1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_bool1, cpu_out_bool1, None)
    # data_info_bool1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_bool1, cpu_out_bool1)
    dispatch_workflow(run_param, data_info_bool1)
    data_info_bool2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_bool2, cpu_out_bool1, None)
    # data_info_bool2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_bool2, cpu_out_bool1)
    dispatch_workflow(run_param, data_info_bool2)
    data_info_int1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_int1, cpu_out_int1, None)
    # data_info_int1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_int1, cpu_out_int1)
    dispatch_workflow(run_param, data_info_int1)
    data_info_int2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_int2, cpu_out_int1, None)
    # data_info_int2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_int2, cpu_out_int1)
    dispatch_workflow(run_param, data_info_int2)
    data_info_float1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_float1, cpu_out_float1, None)
    # data_info_float1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_float1, cpu_out_float1)
    dispatch_workflow(run_param, data_info_float1)
    data_info_float2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_float2, cpu_out_float1, None)
    # data_info_float2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_float2, cpu_out_float1)
    dispatch_workflow(run_param, data_info_float2)
    data_info_str1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_str1, cpu_out_str1, None)
    # data_info_str1 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_str1, cpu_out_str1)
    dispatch_workflow(run_param, data_info_str1)
    data_info_str2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_str2, cpu_out_str1, None)
    # data_info_str2 = DisPatchDataInfo(cpu_args, {}, [[],[]], func, npu_out_cpu_str2, cpu_out_str1)
    dispatch_workflow(run_param, data_info_str2)

if __name__ =='__main__':
    
    compare(dump_path)