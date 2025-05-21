/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "CommunicationBaseAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {

bool CommunicationBaseAnalyzer::GenerateAdvisor(CommunicationAdvisorInfo &info, const std::string &clusterPath)
{
    if (!QueryAdvisorData(clusterPath)) {
        return false;
    }
    ComputeStatistics();
    AssembleAdvisor(info);
    return true;
}
}
}
}