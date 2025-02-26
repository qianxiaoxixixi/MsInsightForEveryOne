/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "MindSpeedParallelStrategyAlgorithm.h"
namespace Dic::Module::Summary {
MindSpeedParallelStrategyAlgorithm::MindSpeedParallelStrategyAlgorithm() = default;

MindSpeedParallelStrategyAlgorithm::~MindSpeedParallelStrategyAlgorithm() = default;

void MindSpeedParallelStrategyAlgorithm::UpdateOrderAndParallelSize()
{
    updatedOrder = paraOrder;
    updatedOrder.erase(std::remove_if(updatedOrder.begin(), updatedOrder.end(), [this](const std::string& group) {
        return !(paraDetailsMap[group].isShown);
        }), updatedOrder.end());
    parallelSize.clear();
    for (const auto& para : updatedOrder) {
        parallelSize.push_back(paraDetailsMap[para].size);
    }
    // todo: get updatedOrderWithEp and parallelSizeWithEp for connections
}

bool MindSpeedParallelStrategyAlgorithm::UpdateParallelDimension(const std::string &tmpDimension,
    const ParallelStrategyConfig &tmpConfig, std::string &err)
{
    CalStrategyConfig(tmpDimension, tmpConfig);
    if (tmpConfig.algorithm == MINDSPEED_TP_CP_EP_DP_PP_ALG) {
        paraOrder = {TP_PARA, CP_PARA, DP_PARA, PP_PARA};
    } else {
        err = "Failed to update parallel view. Unexpected algorithm.";
        return false;
    }
    paraOrderWithEp = paraOrder;
    paraOrderWithEp.insert(paraOrderWithEp.begin() + epPosPpLast, EP_PARA);

    bool res = UpdateShowMap(err);
    if (res) {
        // 根据 paraDetailsMap[para].isShown 删除不存在的通信域
        UpdateOrderAndParallelSize();
        // 计算当前元素总数
        UpdateElementSize();
    }
    return res;
}

bool MindSpeedParallelStrategyAlgorithm::GenerateArrangementByDimension(std::string &err)
{
    ClearArrangementData();
    SetIndicatorAttr();
    std::unordered_map<std::string, uint32_t> indexAttributes;
    for (const auto& para : paraOrderWithEp) {
        indexAttributes[para + STR_INDEX] = 0;
    }
    // get arrangements
    for (uint32_t index = 0; index < elementSize; index++) {
        GetPerArrangement(index, indexAttributes);
    }
    if (!GetConnectionsByTokenList(err)) {
        return false;
    }
    return true;
}

void MindSpeedParallelStrategyAlgorithm::GetPerArrangement(uint32_t index,
    std::unordered_map<std::string, uint32_t> &indexAttributes)
{
    Element element;
    element.index = index;
    if (index != 0) {
        UpdateIndexAttributes(indexAttributes);
    }
    element.indexAttributes = indexAttributes;
    element.name = GetElementName(indexAttributes);
    element.position = GetElementPosition(indexAttributes);
    element.ranks = GetElementContainRanks(index, indexAttributes);
    data.arrangements.push_back(element);
}

void MindSpeedParallelStrategyAlgorithm::UpdateIndexAttributes(
    std::unordered_map<std::string, uint32_t> &indexAttributes)
{
    // 由低层次到高层次遍历各并行域，依次检查是否需要进位
    std::string curIndex;
    for (const auto& curPara : paraOrder) {
        // 未达到size-1，无需进位
        if (indexAttributes[curPara + STR_INDEX] < paraDetailsMap[curPara].size - 1) {
            indexAttributes[curPara + STR_INDEX]++;
            break;
        }
        // 达到size-1，当前位置0，检查下一位
        if (curPara != paraOrder.back()) {
            indexAttributes[curPara + STR_INDEX] = 0;
        }
    }
    // 添加epIndex, 前端入参已校验，分母不可能为零, dpSize * cpSize 一定能被epSize整除
    uint32_t epScale = paraDetailsMap[DP_PARA].size * paraDetailsMap[CP_PARA].size / paraDetailsMap[EP_PARA].size;
    indexAttributes[EP_INDEX] = indexAttributes[DP_INDEX] / epScale;
}

std::vector<uint32_t> MindSpeedParallelStrategyAlgorithm::GetElementContainRanks(uint32_t index,
    std::unordered_map<std::string, uint32_t> &attrs)
{
    std::vector<uint32_t> ranks{};
    if (wordSize <= 1) {
        return ranks;
    }
    if (dimension == DIMENSIONS_TP) {
        ranks.emplace_back(index);
        return ranks;
    }

    uint32_t ppIndexMin = 0;
    uint32_t ppIndexMax = strategyConfig.ppSize;
    uint32_t cpIndexMin = 0;
    uint32_t cpIndexMax = strategyConfig.cpSize;

    if (dimension == DIMENSIONS_CP) {
        cpIndexMin = attrs[CP_INDEX];
        cpIndexMax = cpIndexMin + 1;
        ppIndexMin = attrs[PP_INDEX];
        ppIndexMax = ppIndexMin + 1;
    } else if (dimension == DIMENSIONS_PP) {
        ppIndexMin = attrs[PP_INDEX];
        ppIndexMax = ppIndexMin + 1;
    }

    for (uint32_t ppIndex = ppIndexMin; ppIndex < ppIndexMax; ++ppIndex) {
        for (uint32_t cpIndex = cpIndexMin; cpIndex < cpIndexMax; ++cpIndex) {
            for (uint32_t tpIndex = 0; tpIndex < strategyConfig.tpSize; ++tpIndex) {
                uint32_t rank = tpCpDpSize * ppIndex + tpCpSize * attrs[DP_INDEX] + tpSize * cpIndex + tpIndex;
                ranks.emplace_back(rank);
            }
        }
    }
    return ranks;
}

bool MindSpeedParallelStrategyAlgorithm::GetConnectionsByTokenList(std::string &err)
{
    return true;
}

void MindSpeedParallelStrategyAlgorithm::SetIndicatorAttr()
{
    if (dimension == DIMENSIONS_TP) {
        SetTpIndicatorAttr();
    } else if (dimension == DIMENSIONS_CP) {
        SetCpIndicatorAttr();
    } else if (dimension == DIMENSIONS_PP) {
        SetPpIndicatorAttr();
    } else {
        SetDpIndicatorAttr();
    }
}

std::vector<Connection> MindSpeedParallelStrategyAlgorithm::GetAllCommunicationGroups(std::string &err)
{
    return {};
}

void MindSpeedParallelStrategyAlgorithm::CalAdviceInfo(const std::string &dimension, std::vector<std::string> &advices,
    std::vector<IndicatorDataStruct> &indicatorData)
{
}

bool MindSpeedParallelStrategyAlgorithm::GetPerformanceIndicatorByDimension(
    const GetPerformanceIndicatorParam &performanceParams,
    const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
    std::vector<IndicatorDataStruct> &indicatorData,
    std::string& err)
{
    return true;
}
}