/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "RetransmissionAnalyzer.h"
#include "DataBaseManager.h"
#include "ServerLog.h"

namespace Dic {
namespace Module {
namespace Communication {
// LCOV_EXCL_BR_START
bool RetransmissionAnalyzer::QueryAdvisorData(const std::string &clusterPath)
{
    auto database = Timeline::DataBaseManager::Instance().GetClusterDatabase(clusterPath);
    if (!database || !database->QueryRetransmissionAnalyzerData(data)) {
        Server::ServerLog::Error("Failed to query retransmission analyzer data.");
        return false;
    }
    return true;
}

void RetransmissionAnalyzer::ComputeStatistics()
{
    for (const auto &element : data) {
        if (element.minElapseTime < MIN_RETRANSMISSION_TIME) {
            continue;
        }
        if (element.maxRDMATransitTime > MIN_RETRANSMISSION_TIME) {
            RetransmissionAnalyzerStatistics info{element.groupId, element.opName};
            statistics.emplace_back(info);
        }
    }
}

void RetransmissionAnalyzer::AssembleAdvisor(Dic::Protocol::CommunicationAdvisorInfo &info)
{
    info.name = RETRANSMISSION_ANALYZER_TITLE;
    info.statistics.insert({"name", {}});
    info.statistics.insert({"group name", {}});
    for (const auto &item : statistics) {
        info.statistics["group name"].emplace_back(item.groupName);
        info.statistics["name"].emplace_back(item.opName);
    }
}
// LCOV_EXCL_BR_STOP
}
}
}