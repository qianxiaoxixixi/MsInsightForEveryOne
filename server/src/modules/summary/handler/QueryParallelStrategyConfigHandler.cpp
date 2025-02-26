/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <memory>
#include "MegatronParallelStrategyAlgorithm.h"
#include "MindSpeedParallelStrategyAlgorithm.h"
#include "WsSessionManager.h"
#include "SummaryProtocolRequest.h"
#include "SummaryProtocolResponse.h"
#include "QueryParallelStrategyConfigHandler.h"

namespace Dic::Module::Summary {
using namespace Dic::Server;
bool QueryParallelStrategyConfigHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    auto &request = dynamic_cast<QueryParallelStrategyRequest &>(*requestPtr.get());
    std::unique_ptr<QueryParallelStrategyResponse> responsePtr = std::make_unique<QueryParallelStrategyResponse>();
    QueryParallelStrategyResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    SetResponseResult(response, true);
    WsSession &session = *WsSessionManager::Instance().GetSession();
    auto database = Timeline::DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    if (database == nullptr || !database->QueryParallelStrategyConfig(response.config, response.level)) {
        SetResponseResult(response, false);
        ServerLog::Error("Failed to query parallel strategy config.");
        session.OnResponse(std::move(responsePtr));
        return false;
    }
    if (!response.IsValid()) {
        response.SetDefault();
    }
    std::string errMsg;
    if (!ParallelStrategyAlgorithmManager::Instance().AddOrUpdateAlgorithm(
        database->GetDbPath(), response.config, errMsg)) {
        SendResponse(std::move(responsePtr), false, errMsg);
        return false;
    }
    // 如果存在baseline，则对baseline进行同样的设置
    auto baselineDatabase = Timeline::DataBaseManager::Instance().GetClusterDatabase(BASELINE);
    if (baselineDatabase != nullptr && !ParallelStrategyAlgorithmManager::Instance().AddOrUpdateAlgorithm(
        baselineDatabase->GetDbPath(), response.config, errMsg)) {
        SendResponse(std::move(responsePtr), false, errMsg);
        return false;
    }
    session.OnResponse(std::move(responsePtr));
    return true;
}
}