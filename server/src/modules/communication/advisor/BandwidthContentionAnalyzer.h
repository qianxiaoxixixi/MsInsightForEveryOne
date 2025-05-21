/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_BANDWIDTHCONTENTION_ANALYZER_H
#define PROFILER_SERVER_BANDWIDTHCONTENTION_ANALYZER_H

#include "CommunicationBaseAnalyzer.h"
#include "ClusterDef.h"

namespace Dic {
namespace Module {
namespace Communication {
using namespace Protocol;
using namespace Dic::Module;

const double BANDWIDTH_CONTENTION_ANALYZER_THRESHOLD = 14.4;

struct BandwidthContentionAnalyzerStatistics {
    std::string name;
    double duration;
    double bandwidth;
};

class BandwidthContentionAnalyzer : public CommunicationBaseAnalyzer {
public:
    BandwidthContentionAnalyzer() = default;
    ~BandwidthContentionAnalyzer() override = default;
    bool QueryAdvisorData(const std::string& clusterPath) override;
    void ComputeStatistics() override;
    void AssembleAdvisor(CommunicationAdvisorInfo &info) override;
private:
    BandwidthContentionData data;
    std::vector<BandwidthContentionAnalyzerStatistics> statistics;
};
} // end of namespace Communication
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_BANDWIDTHCONTENTION_ANALYZER_H
