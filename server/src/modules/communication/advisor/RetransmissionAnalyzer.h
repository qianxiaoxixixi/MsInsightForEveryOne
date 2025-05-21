/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_COMMUNICATION_RETRANSMISSION_H
#define PROFILER_SERVER_COMMUNICATION_RETRANSMISSION_H

#include "CommunicationBaseAnalyzer.h"
#include "ClusterDef.h"

namespace Dic {
namespace Module {
namespace Communication {
using namespace Protocol;
using namespace Dic::Module;

const double MIN_RETRANSMISSION_TIME = 4000.0;

struct RetransmissionAnalyzerStatistics {
    std::string groupName;
    std::string opName;
};

class RetransmissionAnalyzer : public CommunicationBaseAnalyzer {
public:
    RetransmissionAnalyzer() = default;
    ~RetransmissionAnalyzer() override = default;
    bool QueryAdvisorData(const std::string &clusterPath) override;
    void ComputeStatistics() override;
    void AssembleAdvisor(CommunicationAdvisorInfo &info) override;
private:
    std::vector<RetransmissionClassificationInfo> data;
    std::vector<RetransmissionAnalyzerStatistics> statistics;
};
} // end of namespace Communication
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_COMMUNICATION_RETRANSMISSION_H
