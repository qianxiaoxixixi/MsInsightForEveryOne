/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "CommunicationBaseAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {

bool CommunicationBaseAnalyzer::GenerateAdvisor(Dic::Protocol::CommunicationAdvisorInfo &info)
{
    if (!QueryAdvisorData()) {
        return false;
    }
    ComputeStatistics();
    AssembleAdvisor(info);
    return true;
}
}
}
}