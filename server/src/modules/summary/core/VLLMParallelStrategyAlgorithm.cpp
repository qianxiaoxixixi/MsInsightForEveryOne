/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "VLLMParallelStrategyAlgorithm.h"

namespace Dic::Module::Summary {

VLLMParallelStrategyAlgorithm::VLLMParallelStrategyAlgorithm() = default;

VLLMParallelStrategyAlgorithm::~VLLMParallelStrategyAlgorithm() = default;

bool VLLMParallelStrategyAlgorithm::UpdateParallelDimension(const std::string& tmpDimension,
    const ParallelStrategyConfig &tmpConfig, std::string &err)
{
    // vLLM也可复用Base类中的计算逻辑，等价于cpSize恒为1
    CalStrategyConfig(tmpDimension, tmpConfig);
    if (tmpConfig.algorithm == VLLM_TP_PP_DP_EP_ALG) {
        paraOrder = {TP_PARA, PP_PARA, DP_PARA};
        paraOrderWithEp = {TP_PARA, PP_PARA, DP_PARA, EP_PARA};
    } else {
        err = "Failed to update parallel view. Unexpected algorithm for the vLLM.";
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

/**
 * world_size = pp_size * tp_size * dp_size
                = pp_size * tp_size * innerDP_size * externalDP_size （非MOE层）
                = pp_size * ep_size * externalDP_size （MOE层）
	ep_size = tp_size * innerDP_size = (tp_size * dp_size) / externalDP_size
    即innerDP_size = ep_size / tp_size
    externalDP_size = dp_size / innerDP_size
    前端入参已校验, tp_size * dp_size能被ep_size整除，且ep_size能被tp_size整除
 * @param config
 */
void VLLMParallelStrategyAlgorithm::SetStrategyConfig(const ParallelStrategyConfig& config)
{
    BaseParallelStrategyAlgorithm::SetStrategyConfig(config);
    // 前端入参已校验, 无除零、整数溢出风险
    innerDpSize = static_cast<uint32_t>(config.epSize / config.tpSize);
    externalDpSize = static_cast<uint32_t>(config.dpSize / innerDpSize);
}

void VLLMParallelStrategyAlgorithm::UpdateOrderAndParallelSize()
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
    // 若epSize不为1，处理MOE层，根据源码，可将其拆解为TP-PP-innerDP-externalDP，其中EP通信域等效为TP-innerDP混合通信域
    // updatedOrderWithEp仅影响连线生成，不影响布局
    updatedOrderWithEp = {TP_PARA, PP_PARA};
    // 根据 paraDetailsMap[para].isShown删除size = 1的通信域
    updatedOrderWithEp.erase(std::remove_if(updatedOrderWithEp.begin(), updatedOrderWithEp.end(),
        [this](const std::string& group) { return !(paraDetailsMap[group].isShown); }),
        updatedOrderWithEp.end());
    parallelSizeWithEp.clear();
    for (const auto& para : updatedOrderWithEp) {
        parallelSizeWithEp.push_back(paraDetailsMap[para].size);
    }
    updatedOrderWithEp.push_back(INNER_DP_GROUP);
    updatedOrderWithEp.push_back(EXTERNAL_DP_GROUP);
    parallelSizeWithEp.push_back(innerDpSize);
    parallelSizeWithEp.push_back(externalDpSize);
}
}