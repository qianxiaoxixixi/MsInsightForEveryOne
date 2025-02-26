/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "BaseParallelStrategyAlgorithm.h"

namespace Dic::Module::Summary {
void BaseParallelStrategyAlgorithm::ClearStrategyConfigCache()
{
    // arrangements data
    data.size = 0;
    data.indicators.clear();
    data.arrangements.clear();
    data.connections.clear();

    elementSize = 1;
    foldedTpSize = 1;
    foldedTpCpSize = 1;
    foldedTpCpDpSize = 1;
    foldedTpCpPpSize = 1;

    paraOrder.clear();
    paraOrderWithEp.clear();
    paraDetailsMap.clear();
    updatedOrder.clear();
    updatedOrderWithEp.clear();
    parallelSize.clear();
    parallelSizeWithEp.clear();

    // performance data
    wordSize = 1;
    tpSize = 1;
    tpCpSize = 1;
    tpCpDpSize = 1;
    tpCpPpSize = 1;
    reduceTpMax.clear();
    reduceTpMin.clear();
    reducePpStatistic.clear();
    reduceCpMax.clear();
    reduceCpMin.clear();
}

void BaseParallelStrategyAlgorithm::SetStrategyConfig(const ParallelStrategyConfig& config)
{
    strategyConfig = config;
}

ParallelStrategyConfig BaseParallelStrategyAlgorithm::GetStrategyConfig()
{
    return strategyConfig;
}

ArrangementAndConnectionData BaseParallelStrategyAlgorithm::GetArrangementData()
{
    return data;
}

void BaseParallelStrategyAlgorithm::CalStrategyConfig(const std::string &tmpDimension,
    const ParallelStrategyConfig &tmpConfig)
{
    strategyConfig = tmpConfig;
    dimension = tmpDimension;
    tpSize = tmpConfig.tpSize;
    tpCpSize = tpSize * tmpConfig.cpSize;
    tpCpDpSize = tpCpSize * tmpConfig.dpSize;
    tpCpPpSize = tpCpSize * tmpConfig.ppSize;
    // 前端入参已校验，无整数溢出风险
    wordSize = tpCpSize * tmpConfig.dpSize * tmpConfig.ppSize;
}

int64_t BaseParallelStrategyAlgorithm::GetParallelSizeByType(const std::string& type) const
{
    if (type == DP_PARA) {
        return strategyConfig.dpSize;
    } else if (type == EP_PARA) {
        return strategyConfig.epSize;
    } else if (type == PP_PARA) {
        return strategyConfig.ppSize;
    } else if (type == TP_PARA) {
        return strategyConfig.tpSize;
    } else if (type == CP_PARA) {
        return strategyConfig.cpSize;
    }
    // 默认值为1，表征没有启用对应的并行方式
    return 1;
}

bool BaseParallelStrategyAlgorithm::UpdateShowMap(std::string &err)
{
    // 按层次关闭showMap, 被关闭的层次size置1，接下来就不用感知区别不同层次size的影响了
    for (const auto& para : paraOrderWithEp) {
        paraDetailsMap[para].isShown = false;
        paraDetailsMap[para].size = 1;
    }
    SetParaDetail(EP_PARA, strategyConfig.epSize);
    SetParaDetail(DP_PARA, strategyConfig.dpSize);
    if (dimension == DIMENSIONS_DP) {
        return true;
    }
    SetParaDetail(PP_PARA, strategyConfig.ppSize);
    if (dimension == DIMENSIONS_PP) {
        return true;
    }
    SetParaDetail(CP_PARA, strategyConfig.cpSize);
    if (dimension == DIMENSIONS_CP) {
        return true;
    }
    SetParaDetail(TP_PARA, strategyConfig.tpSize);
    if (dimension == DIMENSIONS_TP) {
        return true;
    }
    err = "Failed to update show map for parallel view. Unexpected dimension.";
    return false;
}

void BaseParallelStrategyAlgorithm::SetParaDetail(const std::string &para, int64_t size)
{
    // 参数为1，该并行域未生效
    if (size == 1) {
        return;
    }
    paraDetailsMap[para].isShown = true;
    paraDetailsMap[para].size = size;
}

void BaseParallelStrategyAlgorithm::UpdateElementSize()
{
    // 前端入参已校验，无整数溢出风险
    foldedTpSize = paraDetailsMap[TP_PARA].size;
    foldedTpCpSize = foldedTpSize * paraDetailsMap[CP_PARA].size;
    foldedTpCpPpSize = foldedTpCpSize * paraDetailsMap[PP_PARA].size;
    foldedTpCpDpSize = foldedTpCpSize * paraDetailsMap[DP_PARA].size;
    elementSize = foldedTpCpDpSize * paraDetailsMap[PP_PARA].size;
    data.size = elementSize;
}

std::string BaseParallelStrategyAlgorithm::GetElementName(
    std::unordered_map<std::string, uint32_t> &indexAttributes)
{
    std::string name;
    for (const auto& para : LAYOUT) {
        if (paraDetailsMap[para].isShown && GetParallelSizeByType(para) > 1) {
            name += para;
            name += std::to_string(indexAttributes[para + STR_INDEX]);
            name += "-";
        }
    }
    if (!name.empty()) {
        name.pop_back();
    }
    return name;
}

Position BaseParallelStrategyAlgorithm::GetElementPosition(
    std::unordered_map<std::string, uint32_t>& indexAttributes) const
{
    Position position;
    // 前端入参已校验，无整数溢出风险
    position.x = indexAttributes[DP_INDEX] * foldedTpCpSize + indexAttributes[CP_INDEX] * foldedTpSize
                 + indexAttributes[TP_INDEX];
    position.y = indexAttributes[PP_INDEX];
    return position;
}

void BaseParallelStrategyAlgorithm::ClearArrangementData()
{
    data.indicators.clear();
    data.arrangements.clear();
    data.connections.clear();
}

// 对于TP View，展示全部原始数据，及step_trace_time中的内容
void BaseParallelStrategyAlgorithm::SetTpIndicatorAttr()
{
    uint8_t index = 0;
    // 总计算、总通信不在柱状图中显示，而是通过掩盖和未掩盖的堆叠形成
    data.indicators.emplace_back(
        index++, KEY_TOTAL_COMPUTING_TIME, VALUE_TOTAL_COMPUTING_TIME, true, false, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(
        index++, KEY_TOTAL_COMMUNICATION, VALUE_TOTAL_COMMUNICATION, true, false, true, BAR_CHART, "", TIME_AXIS);

    // 参与stack堆叠：预处理、纯计算、重叠、纯通信、下发
    data.indicators.emplace_back(index++, KEY_PURE_COMPUTING_TIME, VALUE_COMPUTING_NOT_OVERLAPPED,
                                 true, true, true, BAR_CHART, TIME_STACK, TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_OVERLAPPED, VALUE_COMMUNICATION_OVERLAPPED,
                                 true, true, true, BAR_CHART, TIME_STACK, TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_NOT_OVERLAPPED, VALUE_COMMUNICATION_NOT_OVERLAPPED,
                                 true, true, true, BAR_CHART, TIME_STACK, TIME_AXIS);
    data.indicators.emplace_back(
        index++, KEY_FREE_TIME, VALUE_FREE_TIME, true, true, true, BAR_CHART, TIME_STACK, TIME_AXIS);
    data.indicators.emplace_back(
        index++, KEY_PREPARING_TIME, VALUE_PREPARING_TIME, true, true, true, BAR_CHART, TIME_STACK, TIME_AXIS);

    // Communication(Not Overlapped and Exclude Receive)
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_NOT_OVERLAPPED_AND_RECEIVE,
        VALUE_COMMUNICATION_NOT_OVERLAPPED_AND_RECEIVE, true, false, false, BAR_CHART, "", TIME_AXIS);

    // stage and bubble
    data.indicators.emplace_back(
        index++, KEY_STAGE_TIME, VALUE_STAGE_TIME, true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(
        index++, KEY_BUBBLE_TIME, VALUE_BUBBLE_TIME, true, false, false, BAR_CHART, "", TIME_AXIS);

    // ratio
    data.indicators.emplace_back(
        index++, KEY_COMPUTING_RATIO, VALUE_COMPUTING_RATIO, false, true, true, LINE_CHART, "", RATIO_AXIS);
    data.indicators.emplace_back(
        index++, KEY_COMMUNICATION_RATIO, VALUE_COMMUNICATION_RATIO, false, true, true, LINE_CHART, "", RATIO_AXIS);
}

// 对于CP Dimension，通过展示一个TP域内计算/通信等时间的统计值，包括最大值、最小值、极差，来分析CP域的性能瓶颈
// 对于通信，可关注最小值，有时通信最小的点，才是瓶颈所在
// 通过极差，可以反映TP域内各卡计算、通信是否均衡，尤其是计算
void BaseParallelStrategyAlgorithm::SetCpIndicatorAttr()
{
    uint8_t index = 0;
    data.indicators.emplace_back(index++, KEY_TOTAL_COMPUTING_TIME + KEY_MAX_SUFFIX,
        VALUE_MAX + VALUE_TOTAL_COMPUTING_TIME, true, true, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMMUNICATION + KEY_MAX_SUFFIX,
        VALUE_MAX + VALUE_TOTAL_COMMUNICATION, true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_FREE_TIME + KEY_MAX_SUFFIX, VALUE_MAX + VALUE_FREE_TIME,
        true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_NPU_TIME + KEY_MAX_SUFFIX, VALUE_MAX + VALUE_NPU_TIME,
        true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMPUTING_TIME + KEY_MIN_SUFFIX,
        VALUE_MIN + VALUE_TOTAL_COMPUTING_TIME, true, false, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMMUNICATION + KEY_MIN_SUFFIX,
        VALUE_MIN + VALUE_TOTAL_COMMUNICATION, true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_FREE_TIME + KEY_MIN_SUFFIX, VALUE_MIN + VALUE_FREE_TIME,
        true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_NPU_TIME + KEY_MIN_SUFFIX, VALUE_MIN + VALUE_NPU_TIME,
        true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMPUTING_TIME + KEY_RANGE_SUFFIX,
        VALUE_TOTAL_COMPUTING_TIME + VALUE_RANGE, true, false, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMMUNICATION + KEY_RANGE_SUFFIX,
        VALUE_TOTAL_COMMUNICATION + VALUE_RANGE, true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_FREE_TIME + KEY_RANGE_SUFFIX, VALUE_FREE_TIME + VALUE_RANGE,
        true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_NPU_TIME + KEY_RANGE_SUFFIX, VALUE_NPU_TIME + VALUE_RANGE,
        true, false, false, BAR_CHART, "", TIME_AXIS);

    // 通信掩盖、通信未掩盖
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_NOT_OVERLAPPED + KEY_MAX_SUFFIX,
        VALUE_MAX + VALUE_COMMUNICATION_NOT_OVERLAPPED, true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_NOT_OVERLAPPED + KEY_MIN_SUFFIX,
        VALUE_MIN + VALUE_COMMUNICATION_NOT_OVERLAPPED, true, false, false, BAR_CHART, "", TIME_AXIS);
}

// 对于PP Dimension，通过展示一个TP&CP域内计算/通信等时间的统计值，包括最大值、最小值、极差，来分析PP域的性能瓶颈
// 对于通信，可关注最小值，有时通信最小的点，才是瓶颈所在
// 通过极差，可以反映TP&CP域内各卡计算、通信是否均衡，尤其是计算
void BaseParallelStrategyAlgorithm::SetPpIndicatorAttr()
{
    uint8_t index = 0;
    // 总计算、总通信、下发、npu总时间, 默认显示总计算
    data.indicators.emplace_back(index++, KEY_TOTAL_COMPUTING_TIME + KEY_MAX_SUFFIX,
        VALUE_MAX + VALUE_TOTAL_COMPUTING_TIME, true, true, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMMUNICATION + KEY_MAX_SUFFIX,
        VALUE_MAX + VALUE_TOTAL_COMMUNICATION, true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_FREE_TIME + KEY_MAX_SUFFIX, VALUE_MAX + VALUE_FREE_TIME,
        true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_NPU_TIME + KEY_MAX_SUFFIX, VALUE_MAX + VALUE_NPU_TIME,
        true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMPUTING_TIME + KEY_MIN_SUFFIX,
        VALUE_MIN + VALUE_TOTAL_COMPUTING_TIME, true, false, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMMUNICATION + KEY_MIN_SUFFIX,
        VALUE_MIN + VALUE_TOTAL_COMMUNICATION, true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_FREE_TIME + KEY_MIN_SUFFIX, VALUE_MIN + VALUE_FREE_TIME,
        true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_NPU_TIME + KEY_MIN_SUFFIX, VALUE_MIN + VALUE_NPU_TIME,
        true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMPUTING_TIME + KEY_RANGE_SUFFIX,
        VALUE_TOTAL_COMPUTING_TIME + VALUE_RANGE, true, false, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_TOTAL_COMMUNICATION + KEY_RANGE_SUFFIX,
        VALUE_TOTAL_COMMUNICATION + VALUE_RANGE, true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_FREE_TIME + KEY_RANGE_SUFFIX, VALUE_FREE_TIME + VALUE_RANGE,
        true, false, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_NPU_TIME + KEY_RANGE_SUFFIX, VALUE_NPU_TIME + VALUE_RANGE,
        true, false, false, BAR_CHART, "", TIME_AXIS);

    // 通信掩盖、通信未掩盖
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_NOT_OVERLAPPED + KEY_MAX_SUFFIX,
        VALUE_MAX + VALUE_COMMUNICATION_NOT_OVERLAPPED, true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, KEY_COMMUNICATION_NOT_OVERLAPPED + KEY_MIN_SUFFIX,
        VALUE_MIN + VALUE_COMMUNICATION_NOT_OVERLAPPED, true, false, false, BAR_CHART, "", TIME_AXIS);
}

// 对于DP Dimension，通过展示一个TP&CP域内计算/通信等时间的统计值，包括最大值、最小值、极差，来分析PP域的性能瓶颈
void BaseParallelStrategyAlgorithm::SetDpIndicatorAttr()
{
    uint8_t index = 0;
    // 总计算、总通信, 默认显示总计算
    data.indicators.emplace_back(index++, VALUE_SUM_OF_MAX + KEY_TOTAL_COMPUTING_TIME,
        VALUE_SUM_OF_MAX + VALUE_TOTAL_COMPUTING_TIME, true, true, true, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, VALUE_SUM_OF_MAX + KEY_TOTAL_COMMUNICATION,
        VALUE_SUM_OF_MAX + VALUE_TOTAL_COMMUNICATION, true, true, false, BAR_CHART, "", TIME_AXIS);
    data.indicators.emplace_back(index++, VALUE_SUM_OF_MAX + KEY_FREE_TIME, VALUE_SUM_OF_MAX + VALUE_FREE_TIME,
        true, true, false, BAR_CHART, "", TIME_AXIS);
    // 通信掩盖、通信未掩盖、下发
    data.indicators.emplace_back(index++, VALUE_SUM_OF_MAX + KEY_COMMUNICATION_NOT_OVERLAPPED,
        VALUE_SUM_OF_MAX + VALUE_COMMUNICATION_NOT_OVERLAPPED, true, false, false, BAR_CHART, "", TIME_AXIS);
}
}
