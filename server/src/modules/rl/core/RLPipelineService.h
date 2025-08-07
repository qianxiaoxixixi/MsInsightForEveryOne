/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_RLPIPELINESERVICE_H
#define PROFILER_SERVER_RLPIPELINESERVICE_H

#include <unordered_map>
#include <vector>
#include <set>
#include "pch.h"
#include "RLProtocolResponse.h"
#include "VirtualTraceDatabase.h"
#include "DomainObject.h"
#include "RLDomainObject.h"

namespace Dic::Module::RL {
class RLPipelineService {
public:
    static bool GetPipelineInfo(Protocol::RLPipelineResponse &response);
private:
    static void Clear();
    static std::vector<Protocol::RLPipelineNode> SearchNode(const std::string &rankId);
    static std::vector<Protocol::RLPipelineNode> QueryMicroBatchByTask(const std::string &fileId,
        const std::vector<Protocol::RLPipelineNode> &taskList);
    static void QueryPipelineByRankId(const std::string &rankIdWithHost);
    static void FillPipelineMap(const std::string &originHostName, const std::string &rankId,
                                const std::vector<Protocol::RLPipelineNode> &pipeline,
                                std::unordered_map<std::string, RLPipelineItem> &targetMap);
    static std::vector<Protocol::RLPipelineNode> QueryMicroBatch(const std::string &fileId, const RLMstxConfig &config,
                                                                 const Protocol::RLPipelineNode &node);
    static void FillAndProcessPipelineData(std::unordered_map<std::string, RLPipelineItem> &pipelineMap,
                                           std::vector<RLPipelineItem> &pipelineData);
    static std::mutex mtx;
    static uint64_t minTime;
    static uint64_t maxTime;
    static std::set<std::string> stageTypeList;
    static std::unordered_map<std::string, RLPipelineItem> taskPipelineMap;
    static std::unordered_map<std::string, RLPipelineItem> microBatchPipelineMap;
};
}
#endif // PROFILER_SERVER_RLPIPELINESERVICE_H
