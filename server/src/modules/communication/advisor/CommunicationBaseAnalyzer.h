/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_COMMUNICATION_BASE_ANALYZER_H
#define PROFILER_SERVER_COMMUNICATION_BASE_ANALYZER_H

#include "CommunicationProtocolResponse.h"

namespace Dic {
namespace Module {
namespace Communication {
using namespace Dic::Protocol;

class CommunicationBaseAnalyzer {
public:
    CommunicationBaseAnalyzer() = default;
    virtual ~CommunicationBaseAnalyzer() = default;
    bool GenerateAdvisor(CommunicationAdvisorInfo &info, const std::string &clusterPath);
    virtual bool QueryAdvisorData(const std::string &clusterPath) = 0;
    virtual void ComputeStatistics() = 0;
    virtual void AssembleAdvisor(CommunicationAdvisorInfo &info) = 0;
};
}
}
}

#endif // PROFILER_SERVER_COMMUNICATION_BASE_ANALYZER_H
