/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "CommunicationAdvisor.h"
#include "PacketAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {

void CommunicationAdvisor::GenerateAdvisor(std::vector<CommunicationAdvisorInfo> &items)
{
    PacketAnalyzer packetAnalyzer;
    CommunicationAdvisorInfo packetAnalyzerInfo;
    if (packetAnalyzer.GenerateAdvisor(packetAnalyzerInfo)) {
        items.emplace_back(packetAnalyzerInfo);
    }
}

}
}
}