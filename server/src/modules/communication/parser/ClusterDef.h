/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_CLUSTER_DEF_H
#define PROFILER_SERVER_CLUSTER_DEF_H

#include <string>
#include <optional>
#include <fstream>
#include "ServerLog.h"

namespace Dic {
namespace Module {
struct CommunicationTimeInfo {
    std::string iterationId;
    std::string stageId;
    std::string rankId;
    std::string opName;
    std::string opSuffix;
    uint64_t startTime = 0;
    double elapseTime = 0;
    double synchronizationTimeRatio = 0;
    double synchronizationTime = 0;
    double transitTime = 0;
    double waitTimeRatio = 0;
    double waitTime = 0;
    double idleTime = 0;
};
struct CommunicationBandWidth {
    std::string iterationId;
    std::string rankId;
    std::string stageId;
    std::string opName;
    std::string opSuffix;
    std::string transportType;
    double bandwidthSize = 0;
    double bandwidthUtilization = 0;
    double largePackageRatio = 0;
    std::string sizeDistribution;
    double transitSize = 0;
    double transitTime = 0;
};
struct StepStatistic {
    std::string rankId;
    std::string stepId;
    std::string stageId;
    double computingTime = 0;
    double pureCommunicationTime = 0;
    double overlapCommunicationTime = 0;
    double communicationTime = 0;
    double freeTime = 0;
    double stageTime = 0;
    double bubbleTime = 0;
    double pureCommunicationExcludeReceiveTime = 0;
    double prepareTime = -1;
    int64_t dpIndex = 0;
    int64_t ppIndex = 0;
    int64_t tpIndex = 0;
};

const std::string PARALLEL_CONFIG_LEVEL_COLLECTED = "collected";
const std::string PARALLEL_CONFIG_LEVEL_PREDICTED = "predicted";
const std::string PARALLEL_CONFIG_LEVEL_CONFIRMED = "confirmed";
const std::string PARALLEL_CONFIG_LEVEL_CONFIGURED = "configured";
const std::string PARALLEL_CONFIG_LEVEL_UNDEFINED = "undefined";

const std::string MEGATRON_LM_TP_DP_PP_ALG = "Megatron-LM(tp-dp-pp)";
const std::string MEGATRON_LM_TP_PP_DP_ALG = "Megatron-LM(tp-pp-dp)";
const int64_t MAX_PARALLEL_SIZE = 255;
const int64_t MAX_PARALLEL_PRODUCT_SIZE = 250000;

struct ParallelStrategyConfig {
    std::string algorithm = MEGATRON_LM_TP_DP_PP_ALG; // megatron-lm tp-dp-pp, megatron-lm tp-pp-dp
    int64_t ppSize{};
    int64_t tpSize{};
    int64_t dpSize{};
    int64_t cpSize = 1;
    int64_t epSize = 1;
    bool CheckParams(std::string &errorMsg) const
    {
        // 检查ppSize, tpSize, dpSize的范围
        if (ppSize <= 0 || ppSize > MAX_PARALLEL_SIZE) {
            errorMsg = "[Summary] PP size must be between 1 and " + std::to_string(MAX_PARALLEL_SIZE);
            return false;
        }
        if (tpSize <= 0 || tpSize > MAX_PARALLEL_SIZE) {
            errorMsg = "[Summary] TP size must be between 1 and " + std::to_string(MAX_PARALLEL_SIZE);
            return false;
        }
        if (dpSize <= 0 || dpSize > MAX_PARALLEL_SIZE) {
            errorMsg = "[Summary] DP size must be between 1 and " + std::to_string(MAX_PARALLEL_SIZE);
            return false;
        }
        if (cpSize <= 0 || cpSize > MAX_PARALLEL_SIZE) {
            errorMsg = "[Summary] CP size must be between 1 and " + std::to_string(MAX_PARALLEL_SIZE);
            return false;
        }
        if (epSize <= 0 || epSize > MAX_PARALLEL_SIZE) {
            errorMsg = "[Summary] EP size must be between 1 and " + std::to_string(MAX_PARALLEL_SIZE);
            return false;
        }
        // 检查三个数的乘积是否小于MAX_PARALLEL_PRODUCT_SIZE(25万)
        if (ppSize * tpSize * dpSize * cpSize > MAX_PARALLEL_PRODUCT_SIZE) {
            errorMsg = "[Summary] The product of PP size, TP size, DP size, and CP size must be less than " +
                       std::to_string(MAX_PARALLEL_PRODUCT_SIZE);
            return false;
        }
        return true;
    }
};

struct ClusterBaseInfo {
    std::string filePath;
    std::string ranks;
    std::string steps;
    std::string ppStages;
    std::string stages;
    long long collectStartTime;
    double collectDuration;
    long long dataSize  =  0;
    ParallelStrategyConfig config;
    std::string level;
};

struct CommunicationMatrixInfo {
    std::string groupId;
    std::string iterationId;
    std::string sortOp;
    std::string opName;
    std::string groupName;
    int srcRank;
    int dstRank;
    std::string transportType;
    double transitSize;
    double transitTime;
    double bandwidth;
};

// cluster_step_trace_time.csv表头
const std::string FIELD_STEP = "Step";
const std::string FIELD_TYPE = "Type";
const std::string FIELD_INDEX = "Index";
const std::string FIELD_COMPUTING = "Computing";
const std::string FIELD_COMMUNICATION_NOT_OVERLAPPED = "Communication(Not Overlapped)";
const std::string FIELD_OVERLAPPED = "Overlapped";
const std::string FIELD_COMMUNICATION = "Communication";
const std::string FIELD_FREE = "Free";
const std::string FIELD_STAGE = "Stage";
const std::string FIELD_BUBBLE = "Bubble";
const std::string FIELD_COMMUNICATION_NOT_OVERLAPPED_AND_RECEIVE = "Communication(Not Overlapped and Exclude Receive)";
const std::string FIELD_PREPARE_TIME = "Preparing";
const std::string FIELD_DP_INDEX = "DP Index";
const std::string FIELD_PP_INDEX = "PP Index";
const std::string FIELD_TP_INDEX = "TP Index";

const std::vector<std::string> VALID_STEP_STATISTICS_HEADERS = {
    FIELD_STEP, FIELD_TYPE, FIELD_INDEX, FIELD_COMPUTING, FIELD_COMMUNICATION_NOT_OVERLAPPED, FIELD_OVERLAPPED,
    FIELD_COMMUNICATION, FIELD_FREE, FIELD_STAGE, FIELD_BUBBLE, FIELD_COMMUNICATION_NOT_OVERLAPPED_AND_RECEIVE
};
const std::vector<std::string> PARALLEL_STRATEGY_HEADERS = {
    FIELD_DP_INDEX, FIELD_PP_INDEX, FIELD_TP_INDEX
};
} // end of namespace Module
} // end of namespace Dic
#endif // PROFILER_SERVER_CLUSTER_DEF_H