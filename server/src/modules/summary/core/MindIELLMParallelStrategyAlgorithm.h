/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_MINDIELLMPARALLELSTRATEGYALGORITHM_H
#define PROFILER_SERVER_MINDIELLMPARALLELSTRATEGYALGORITHM_H

#include "BaseParallelStrategyAlgorithm.h"
#include "ParallelStrategyAlgorithmHelper.h"
#include "ParallelStrategyAlgorithmDef.h"

namespace Dic::Module::Summary {
class MindIELLMParallelStrategyAlgorithm : public BaseParallelStrategyAlgorithm {
public:
    MindIELLMParallelStrategyAlgorithm();
    ~MindIELLMParallelStrategyAlgorithm() override;

    void SetStrategyConfig(const ParallelStrategyConfig& config) override;
    bool UpdateParallelDimension(const std::string& tmpDimension,
                                 const ParallelStrategyConfig& tmpConfig, std::string& err) override;
    bool GenerateArrangementByDimension(std::string &err) override;
    bool GetPerformanceIndicatorByDimension(const GetPerformanceIndicatorParam &performanceParams,
                                            const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
                                            std::vector<IndicatorDataStruct> &indicatorData, std::string& err) override;
    void CalAdviceInfo(const std::string &dimension, std::vector<std::string> &advices,
                       std::vector<IndicatorDataStruct> &indicatorData) override;
    std::vector<Connection> GetAllCommunicationGroups(std::string &err) override;
private:
};
}

#endif // PROFILER_SERVER_MINDIELLMPARALLELSTRATEGYALGORITHM_H
