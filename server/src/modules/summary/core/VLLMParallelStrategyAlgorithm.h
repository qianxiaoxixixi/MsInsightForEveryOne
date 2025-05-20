/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_VLLMPARALLELSTRATEGYALGORITHM_H
#define PROFILER_SERVER_VLLMPARALLELSTRATEGYALGORITHM_H

#include "BaseParallelStrategyAlgorithm.h"
#include "ParallelStrategyAlgorithmHelper.h"
#include "ParallelStrategyAlgorithmDef.h"

namespace Dic::Module::Summary {
class VLLMParallelStrategyAlgorithm : public BaseParallelStrategyAlgorithm {
public:
    VLLMParallelStrategyAlgorithm();
    ~VLLMParallelStrategyAlgorithm() override;
    bool UpdateParallelDimension(const std::string& tmpDimension,
                                 const ParallelStrategyConfig& tmpConfig, std::string& err) override;
    void SetStrategyConfig(const ParallelStrategyConfig& config) override;
private:
    void UpdateOrderAndParallelSize();
    uint32_t innerDpSize = 1;
    uint32_t externalDpSize = 1;
};
}

#endif // PROFILER_SERVER_VLLMPARALLELSTRATEGYALGORITHM_H
