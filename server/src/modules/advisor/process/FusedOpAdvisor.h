/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_FUSEDOPADVISOR_H
#define PROFILER_SERVER_FUSEDOPADVISOR_H

#include "VirtualTraceDatabase.h"
#include "AdvisorProtocolRequest.h"
#include "AdvisorProtocolResponse.h"

namespace Dic::Module::Advisor {
const std::vector<Timeline::FuseableOpRule> FUSEABLE_OPERATER_RULE_LIST = {
    {{"Gelu", "Add"}, "GeluAdd", ""},
    {{"Add", "LayerNorm"}, "AddLayerNorm", ""},
    {{"Add", "LayerNormV3"}, "AddLayerNorm", ""},
    {{"Cast", "LayerNorm", "Cast"}, "LayerNorm", ""},
    {{"Add", "DropOutDoMask", "Add"}, "bias_dropout_add", ""},
    {{"MatMulV2", "Swish", "MatMulV2", "Mul", "MatMulV2"}, "FFN", ""},
    {{"Transpose", "Transpose", "GatherElement", "Transpose"}, "GatherElement", ""},
    {{"Mul", "Slice", "Neg", "Slice", "ConcatD", "Mul", "Add"}, "RotaryMul", ""},
    {{"Mul", "AsStrided", "Neg", "AsStrided", "ConcatD", "Mul", "Add"}, "RotaryMul", ""},
    {{"Mul", "Slice", "Neg", "Slice", "ConcatD", "Cast", "Mul", "Add"}, "RotaryMul", ""},
    {{"Cast", "Square", "MemSet", "ReduceMean", "Add", "Rsqrt", "Mul", "Cast", "Mul"}, "RMSNorm", ""},
    {
        {"Cast", "Square", "ReduceMeanD", "Add", "Rsqrt", "Cast", "Cast", "Mul", "Cast", "Cast", "Mul", "Cast"},
        "RMSNORM",
        ""
    },
};

class FusedOpAdvisor {
public:
    static bool Process(const Protocol::APITypeParams& params, Protocol::OperatorFusionResBody& resBody);
};

} // Dic::Module::Advisor

#endif // PROFILER_SERVER_FUSEDOPADVISOR_H
