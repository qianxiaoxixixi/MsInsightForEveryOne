/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "MindIELLMParallelStrategyAlgorithm.h"

namespace Dic::Module::Summary {

MindIELLMParallelStrategyAlgorithm::MindIELLMParallelStrategyAlgorithm() = default;

MindIELLMParallelStrategyAlgorithm::~MindIELLMParallelStrategyAlgorithm() = default;

void MindIELLMParallelStrategyAlgorithm::SetStrategyConfig(const ParallelStrategyConfig& config)
{
}

bool MindIELLMParallelStrategyAlgorithm::UpdateParallelDimension(const std::string& tmpDimension,
    const ParallelStrategyConfig &tmpConfig, std::string &err)
{
    // MindIE-LLM也可复用Base类中的计算逻辑，等价于cpSize恒为1
    CalStrategyConfig(tmpDimension, tmpConfig);
    if (tmpConfig.algorithm == MINDIE_LLM_TP_DP_EP_PP_MOETP_ALG) {
        paraOrder = {TP_PARA, DP_PARA, PP_PARA};
        paraOrderWithEp = {MOE_TP_PARA, EP_PARA};
    } else {
        err = "Failed to update parallel view. Unexpected algorithm for the MOE algorithm.";
        return false;
    }
    return true;
}

bool MindIELLMParallelStrategyAlgorithm::GenerateArrangementByDimension(std::string &err)
{
    return true;
}

bool MindIELLMParallelStrategyAlgorithm::GetPerformanceIndicatorByDimension(
    const GetPerformanceIndicatorParam &performanceParams,
    const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
    std::vector<IndicatorDataStruct> &indicatorData, std::string& err)
{
    return true;
}

void MindIELLMParallelStrategyAlgorithm::CalAdviceInfo(const std::string &dimension, std::vector<std::string> &advices,
    std::vector<IndicatorDataStruct> &indicatorData)
{
}

std::vector<Connection> MindIELLMParallelStrategyAlgorithm::GetAllCommunicationGroups(std::string &err)
{
    return {};
}
}