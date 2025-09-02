/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_RLPROTOCOLRESPONSE_H
#define PROFILER_SERVER_RLPROTOCOLRESPONSE_H

#include <vector>
#include "ProtocolUtil.h"
#include "ProtocolDefs.h"

namespace Dic::Protocol {

struct RLPipelineNode {
    std::string fileId;
    std::string nodeType;
    uint64_t startTime = 0;
    uint64_t duration = 0;
    std::string name;
    std::string stageType;
};

struct RLPipelineItem {
    std::vector<RLPipelineNode> lists;
    std::string rankId;
    std::string hostName;
};

struct RLPipelineBody {
    uint64_t minTime = UINT64_MAX;
    uint64_t maxTime = 0;
    std::vector<RLPipelineItem> taskData;
    std::vector<RLPipelineItem> microBatchData;
    std::vector<std::string> stageTypeList;
    std::string backendType;
    std::string framework;
};

struct RLPipelineResponse : public Response {
    RLPipelineResponse() : Response(REQ_REQ_RL_PIPELINE) {}
    RLPipelineBody body;
};
}

#endif // PROFILER_SERVER_RLPROTOCOLRESPONSE_H
