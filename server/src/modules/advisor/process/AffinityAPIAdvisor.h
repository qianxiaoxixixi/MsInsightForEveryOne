/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_AFFINITYAPIADVISOR_H
#define PROFILER_SERVER_AFFINITYAPIADVISOR_H

#include <map>
#include <vector>
#include "TimelineProtocolResponse.h"
#include "AdvisorProtocolRequest.h"
#include "AdvisorProtocolResponse.h"

namespace Dic::Module::Advisor {
const std::string API_SEP = "::";
struct AffinityApiData {
    std::vector<std::string> apiList;
    std::string affinityApi;
    std::string note;
};

const std::vector<AffinityApiData> AFFINITY_API_RULE = {
    { std::vector<std::string>{ "gelu" }, "torch_npu.fast_gelu", "" },
    { std::vector<std::string>{ "linear" }, "torch_npu.npu_linear", "" },
    { std::vector<std::string>{ "mish" }, "torch_npu.npu_mish / torch_npu.contrib.module.Mish", "" },
    { std::vector<std::string>{ "silu" }, "torch_npu.npu_silu / torch_npu.contrib.module.SiLU", "" },
    { std::vector<std::string>{ "mul", "sigmoid" }, "torch_npu.npu_silu / torch_npu.contrib.module.SiLU", "" },
    { std::vector<std::string>{ "sigmoid", "mul" }, "torch_npu.npu_silu / torch_npu.contrib.module.SiLU", "" },
    { std::vector<std::string>{ "add", "reciprocal", "mul" }, "optimizer.clip_grad_norm_fused_", "" },
    { std::vector<std::string>{ "slice", "gelu", "mul" }, "torch_npu.npu_geglu", "" },
    { std::vector<std::string>{ "chunk", "gelu", "mul" }, "torch_npu.npu_geglu", "" },
    { std::vector<std::string>{ "slice", "mul", "gelu" }, "torch_npu.npu_geglu", "" },
    { std::vector<std::string>{ "chunk", "mul", "gelu" }, "torch_npu.npu_geglu", "" },
};

class AffinityAPIAdvisor {
public:
    static bool Process(const Protocol::APITypeParams &params, Protocol::AffinityAPIResBody &resBody);

private:
    static std::vector<std::string> GetFirstApiList(const std::vector<AffinityApiData> &affinityApiData);
    static void FilterAffinityApiData(const Protocol::APITypeParams &params,
        const std::vector<Protocol::FlowLocation> &dataList, const std::vector<uint32_t> &indexList,
        Protocol::AffinityAPIResBody &resBody);
    static bool CheckApiSeqWithRule(const std::vector<std::string> &rule,
        const std::vector<Protocol::FlowLocation> &dataList, uint32_t index);
};
} // Dic::Module::Advisor

#endif // PROFILER_SERVER_AFFINITYAPIADVISOR_H
