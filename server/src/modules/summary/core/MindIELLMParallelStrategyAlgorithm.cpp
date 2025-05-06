/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "MindIELLMParallelStrategyAlgorithm.h"

namespace Dic::Module::Summary {

const std::unordered_map<std::string, std::string> MindIELLMParallelStrategyAlgorithm::tokenExceptEp = {
    {DP_GROUP, DP_GROUP}, {TP_GROUP, TP_GROUP}, {PP_GROUP, PP_GROUP}};
const std::unordered_map<std::string, std::string> MindIELLMParallelStrategyAlgorithm::tokenWithEp = {
    {MOE_TP_GROUP, MOE_TP_GROUP_NAME}, {EP_GROUP, EP_GROUP_NAME}};

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
        paraOrderWithEp = {TP_PARA, DP_PARA, PP_PARA, MOE_TP_PARA, EP_PARA};
    } else {
        err = "Failed to update parallel view. Unexpected algorithm for the MOE algorithm.";
        return false;
    }
    bool res = UpdateShowMap(err);
    if (res) {
        // 根据 paraDetailsMap[para].isShown 删除size = 1的通信域
        UpdateOrderAndParallelSize();
        // 计算当前元素总数
        UpdateElementSize();
    }
    return res;
}

void MindIELLMParallelStrategyAlgorithm::UpdateOrderAndParallelSize()
{
    // 根据 paraDetailsMap[para].isShown 删除size = 1的通信域
    updatedOrder = paraOrder;
    updatedOrder.erase(std::remove_if(updatedOrder.begin(), updatedOrder.end(), [this](const std::string& group) {
        return !(paraDetailsMap[group].isShown);
        }), updatedOrder.end());
    parallelSize.clear();
    for (const auto& para : updatedOrder) {
        parallelSize.push_back(paraDetailsMap[para].size);
    }
    if (!paraDetailsMap[EP_PARA].isShown) {
        return;
    }
    // 若epSize不为1，处理moe_tp/ep/pp, 根据 paraDetailsMap[para].isShown删除size = 1的通信域
    // 此处不应将TP/DP纳入，以免影响后续连线生成
    updatedOrderWithEp = {MOE_TP_PARA, EP_PARA, PP_PARA};
    updatedOrderWithEp.erase(std::remove_if(updatedOrderWithEp.begin(), updatedOrderWithEp.end(),
        [this](const std::string& group) { return !(paraDetailsMap[group].isShown); }),
        updatedOrderWithEp.end());
    parallelSizeWithEp.clear();
    for (const auto& para : updatedOrderWithEp) {
        parallelSizeWithEp.push_back(paraDetailsMap[para].size);
    }
}

void MindIELLMParallelStrategyAlgorithm::SetIndicatorAttr()
{
    if (dimension == DIMENSIONS_TP) {
        SetTpIndicatorAttr();
    } else if (dimension == DIMENSIONS_PP) {
        SetPpIndicatorAttr();
    } else if (dimension == DIMENSIONS_DP) {
        SetDpIndicatorAttr();
    } else {
        Server::ServerLog::Error("Failed to set indicator attributes for the MOE algorithm. Unexpected dimension.");
    }
}

void MindIELLMParallelStrategyAlgorithm::UpdateIndexAttributes(
    std::unordered_map<std::string, uint32_t> &indexAttributes)
{
    // 由低层次到高层次遍历各并行域，依次检查是否需要进位
    std::string curIndex;
    // 先更新TP DP PP Index
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
    // 再更新moeTpIndex和epIndex, 目前仅支持全展开视图下返回
    if (dimension != DIMENSIONS_TP) {
        return;
    }
    static std::vector<std::string> updateIndexListForMoe = {MOE_TP_PARA, EP_PARA};
    for (const auto& curPara : updateIndexListForMoe) {
        // 未达到size-1，无需进位
        if (indexAttributes[curPara + STR_INDEX] < paraDetailsMap[curPara].size - 1) {
            indexAttributes[curPara + STR_INDEX]++;
            break;
        }
        // 达到size-1，当前位置0，检查下一位
        indexAttributes[curPara + STR_INDEX] = 0;
    }
}

void MindIELLMParallelStrategyAlgorithm::GetPerArrangement(uint32_t index,
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
    element.ranks = GetElementContainRanks(index, indexAttributes, element.formattedRanks);
    data.arrangements.push_back(element);
}

bool MindIELLMParallelStrategyAlgorithm::GetConnectionsByToken(std::string &err, ParaMode mode)
{
    std::unordered_map<std::string, std::string> tmpToken;
    if (mode == ParaMode::TP_DP_PP) {
        tmpToken = tokenExceptEp;
    } else {
        tmpToken = tokenWithEp;
    }
    // 按paraDetailsMap[group].isShown去除不存在的通信域
    for (const auto& [token, groupName] : tmpToken) {
        bool hasTokenGroup = true;
        std::vector<std::string> parallelGroups = StringUtil::Split(token, "-");
        for (const auto& group : parallelGroups) {
            if (!paraDetailsMap[group].isShown) {
                // 不存在当前含有当前并行域的通信组
                hasTokenGroup = false;
                break;
            }
        }
        if (!hasTokenGroup) {
            continue;
        }
        allGroupsType ranks{};
        if (mode == ParaMode::TP_DP_PP) {
            // 计算tp/dp/pp相关通信域
            ranks = ParallelStrategyAlgorithmHelper::GetAllGroupsRanksByToken(parallelGroups, parallelSize,
                                                                              updatedOrder, wordSize);
        } else {
            // 计算moeTp/ep/pp相关通信域, pp参与连线计算，但不重复添加
            ranks = ParallelStrategyAlgorithmHelper::GetAllGroupsRanksByToken(parallelGroups, parallelSizeWithEp,
                                                                              updatedOrderWithEp, wordSize);
        }
        if (ranks.empty()) {
            err = "Failed to get connections by token list for Megatron. Group name: " + groupName;
            return false;
        }
        for (const auto& rank : ranks) {
            data.connections.emplace_back(groupName, rank, std::vector<std::string>{});
        }
    }
    return true;
}

bool MindIELLMParallelStrategyAlgorithm::GetConnectionsByTokenList(std::string &err)
{
    if (wordSize == 1) {
        err = "Failed to get connections for the MOE algorithm. Parallel strategy configs have not been updated yet.";
        return false;
    }
    // 计算并行通信域, 先处理tp/dp/pp
    if (!GetConnectionsByToken(err, ParaMode::TP_DP_PP)) {
        return false;
    }
    //
    // 计算并行通信域, 处理moeTp/ep/pp, pp connections不重复添加, 目前仅支持全展开视图下生成moe相关连线
    if (dimension == DIMENSIONS_TP && paraDetailsMap[EP_PARA].isShown &&
        !GetConnectionsByToken(err, ParaMode::MOE_TP_EP_PP)) {
        return false;
    }
    if (dimension == DIMENSIONS_TP) {
        allCommunicationGroups = data.connections;
    }
    return true;
}

bool MindIELLMParallelStrategyAlgorithm::GenerateArrangementByDimension(std::string &err)
{
    ClearArrangementData();
    SetIndicatorAttr();
    // 记录并行域坐标，例如dpIndex、tpIndex、ppIndex等
    std::unordered_map<std::string, uint32_t> indexAttributes;

    // tp/dp/pp
    for (const auto& para : paraOrder) {
        indexAttributes[para + STR_INDEX] = 0;
    }
    // moeTp/ep
    indexAttributes[MOE_TP_INDEX] = 0;
    indexAttributes[PP_INDEX] = 0;
    // get arrangements
    for (uint32_t index = 0; index < elementSize; index++) {
        GetPerArrangement(index, indexAttributes);
    }
    // get connections
    if (!GetConnectionsByTokenList(err)) {
        return false;
    }
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