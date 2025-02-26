/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_SERVER_SRC_MODULES_SUMMARY_CORE_MEGATRONPARALLELSTRATEGYALGORITHM_H
#define PROFILER_SERVER_SERVER_SRC_MODULES_SUMMARY_CORE_MEGATRONPARALLELSTRATEGYALGORITHM_H

#include <unordered_map>
#include "SummaryProtocolResponse.h"
#include "BaseParallelStrategyAlgorithm.h"
#include "ParallelStrategyAlgorithmHelper.h"
#include "ParallelStrategyAlgorithmDef.h"

namespace Dic::Module::Summary {

const std::unordered_map<std::string, std::string> tokenNameList = {
    {DP_GROUP, DP_GROUP}, {CP_GROUP, CP_GROUP}, {TP_GROUP, TP_GROUP}, {PP_GROUP, PP_GROUP}, {DP_CP_GROUP, DP_CP_GROUP},
    {MP_GROUP, MP_GROUP_NAME}, {TP_DP_CP_GROUP, TP_DP_CP_GROUP}, {TP_DP_GROUP, TP_DP_GROUP}, {TP_CP_GROUP, TP_CP_GROUP},
    {EP_GROUP, EP_GROUP_NAME}, {TP_EP_GROUP, TP_EP_GROUP_NAME}, {DP_MODULO_EP_GROUP, DP_MODULO_EP_GROUP_NAME},
    {DP_CP_MODULO_EP_GROUP, DP_CP_MODULO_EP_GROUP_NAME}, {MP_EP_GROUP, MP_EP_GROUP_NAME}
};
const std::vector<std::string> independentEpList = {EP_GROUP_NAME, TP_EP_GROUP_NAME, DP_MODULO_EP_GROUP_NAME,
    DP_CP_MODULO_EP_GROUP_NAME, MP_EP_GROUP_NAME};

class MegatronParallelStrategyAlgorithm : public BaseParallelStrategyAlgorithm {
public:
    MegatronParallelStrategyAlgorithm();
    ~MegatronParallelStrategyAlgorithm() override;

    bool UpdateParallelDimension(const std::string &tmpDimension,
                                 const ParallelStrategyConfig &tmpConfig, std::string &err) override;

    bool GenerateArrangementByDimension(std::string &err) override;
    bool GetPerformanceIndicatorByDimension(const GetPerformanceIndicatorParam &performanceParams,
        const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
        std::vector<IndicatorDataStruct> &indicatorData, std::string& err) override;
    void CalAdviceInfo(const std::string &dimension, std::vector<std::string> &advices,
                       std::vector<IndicatorDataStruct> &indicatorData) override;
    // get all communication groups
    std::vector<Connection> GetAllCommunicationGroups(std::string &err) override;
private:
    // get arrangements
    void SetIndicatorAttr();
    void GetPerArrangement(uint32_t index, std::unordered_map<std::string, uint32_t> &indexAttributes);
    void UpdateIndexAttributes(std::unordered_map<std::string, uint32_t> &indexAttributes);
    std::vector<uint32_t> GetElementContainRanks(uint32_t index,
                                                 std::unordered_map<std::string, uint32_t> &indexAttributes);
    // get connections
    void UpdateOrderAndParallelSize();
    bool GetConnectionsByTokenList(std::string &err);
    void GetConnections(Element &curEle);
    void AddConnection(std::vector<Connection> &connections, const std::string &paraType, uint32_t len,
                       uint32_t stepSize, Element &curEle);
    // get performance data
    void SortPerformanceDataByIndex(std::vector<IndicatorDataStruct>& performanceData);
    void CalculatePerformanceDataWithTpDimension(
        const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
        std::vector<IndicatorDataStruct> &performanceResponseData);
    void CalculatePerformanceDataWithCpDimension(std::vector<IndicatorDataStruct> &indicatorData);
    void CalculatePerformanceDataWithPpDimension(std::vector<IndicatorDataStruct> &indicatorData);
    void ReduceTpPerformance(const std::unordered_map<std::uint32_t, StepStatistic> &statistic);
    void GetPerformanceResponseDataWithDpDimension(
        const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
        std::vector<IndicatorDataStruct> &indicatorData);
    void ReducePpPerformanceForPpLast();
    void ReducePpPerformanceForDpLast();
    void ReducePpPerformance(uint32_t startIndex, uint32_t step, uint32_t& dpGroupIdx);
    void ReduceCpPerformance();
    double Reserved3DecimalPlaces(double num);
    void AnalyzePerformanceAdviceWithDpCpPpTpDimension(Protocol::TraceStatistic &max, Protocol::TraceStatistic &min,
                                                       double meanE2ETime, std::vector<std::string> &advices);
    const static inline int epPosDpLast = 3; // tp-cp-pp-ep-dp
};
}
#endif // PROFILER_SERVER_SERVER_SRC_MODULES_SUMMARY_CORE_MEGATRONPARALLELSTRATEGYALGORITHM_H
