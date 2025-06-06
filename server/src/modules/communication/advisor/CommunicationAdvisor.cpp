/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "CommunicationAdvisor.h"
#include "PacketAnalyzer.h"
#include "ByteAlignmentAnalyzer.h"
#include "BandwidthContentionAnalyzer.h"
#include "RetransmissionAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {
// LCOV_EXCL_BR_START
void CommunicationAdvisor::Register()
{
    advisorMap.emplace(PACKET_ANALYZER_TITLE, std::make_unique<PacketAnalyzer>());
    advisorMap.emplace(BYTEALIGNMENT_ANALYZER_TITLE, std::make_unique<ByteAlignmentAnalyzer>());
    advisorMap.emplace(BANDWIDTHCONTENTION_ANALYZER_TITLE, std::make_unique<BandwidthContentionAnalyzer>());
    advisorMap.emplace(RETRANSMISSION_ANALYZER_TITLE, std::make_unique<RetransmissionAnalyzer>());
}

void CommunicationAdvisor::GenerateAdvisor(std::vector<CommunicationAdvisorInfo> &items, const std::string &clusterPath)
{
    for (const auto &analyzer : advisorMap) {
        CommunicationAdvisorInfo info;
        if (analyzer.second->GenerateAdvisor(info, clusterPath)) {
            items.emplace_back(info);
        }
    }
}
// LCOV_EXCL_BR_STOP
}
}
}