/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "DataBaseManager.h"
#include "ServerLog.h"
#include "TraceTime.h"
#include "VirtualTraceDatabase.h"
#include "AclnnOpAdvisor.h"

namespace Dic::Module::Advisor {
using namespace Dic::Server;
bool AclnnOpAdvisor::Process(const Protocol::APITypeParams &params, Protocol::AclnnOperatorResBody &resBody)
{
    auto database = Timeline::DataBaseManager::Instance().GetTraceDatabase(params.rankId);
    if (database == nullptr) {
        ServerLog::Error("Failed to get connection in Aclnn op advice. fileId:", params.rankId);
        return false;
    }
    std::vector<Protocol::KernelBaseInfo> data{};
    Protocol::KernelDetailsParams param = {.orderBy = params.orderBy, .order = params.orderType,
                                           .current = params.currentPage, .pageSize = params.pageSize};
    if (!AclnnOpProcess(database, param, resBody)) {
        return false;
    }

    resBody.size = data.size();
    return true;
}

bool AclnnOpAdvisor::AclnnOpProcess(const std::shared_ptr<Timeline::VirtualTraceDatabase>& database,
    const Protocol::KernelDetailsParams &param, Protocol::AclnnOperatorResBody &resBody)
{
    uint64_t startTime = Timeline::TraceTime::Instance().GetStartTime();
    std::vector<Protocol::KernelBaseInfo> data{};
    if (!database->QueryAclnnOpCountExceedThreshold(param, ACLNN_OP_CNT_THRESHOLD, data, startTime)) {
        ServerLog::Error("Failed to Query Aclnn Op from database which count exceeds .", ACLNN_OP_CNT_THRESHOLD);
        return false;
    }

    for (const auto &item : data) {
        Protocol::AclnnOperatorData one{};
        one.baseInfo.rankId = param.rankId;
        one.baseInfo.startTime = item.startTime;
        one.baseInfo.duration = item.duration;
        one.baseInfo.pid = item.pid;
        one.baseInfo.tid = item.tid;
        one.opName = item.name;
        one.note = "";
        resBody.datas.emplace_back(one);
    }
    return true;
}
} // Dic::Module::Advisor