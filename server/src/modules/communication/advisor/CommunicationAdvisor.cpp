/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "CommunicationAdvisor.h"
#include "PacketAnalyzer.h"
#include "ByteAlignmentAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {

void CommunicationAdvisor::Register()
{
    advisorMap.emplace(PACKET_ANALYZER_TITLE, std::make_unique<PacketAnalyzer>());
    advisorMap.emplace(BYTEALIGNMENT_ANALYZER_TITLE, std::make_unique<ByteAlignmentAnalyzer>());
}

void CommunicationAdvisor::GenerateAdvisor(std::vector<CommunicationAdvisorInfo> &items)
{
    for (const auto &analyzer : advisorMap) {
        CommunicationAdvisorInfo info;
        if (analyzer.second->GenerateAdvisor(info)) {
            items.emplace_back(info);
        }
    }
}

}
}
}