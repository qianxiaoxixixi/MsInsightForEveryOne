/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "TraceTime.h"
#include "QueryParallelismPerformanceHandler.h"
namespace Dic::Module::Summary {
bool QueryParallelismPerformanceHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    auto &request = dynamic_cast<QueryParallelismPerformanceRequest &>(*requestPtr);
    std::unique_ptr<ParallelismPerformanceResponse> responsePtr = std::make_unique<ParallelismPerformanceResponse>();
    ParallelismPerformanceResponse &response = *responsePtr;
    SetBaseResponse(request, response);
    // check request parameter
    std::string err;
    if (!request.params.config.CheckParams(err)) {
        SendResponse(std::move(responsePtr), false, err);
        return false;
    }
    auto database = Timeline::DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    if (database == nullptr) {
        SendResponse(std::move(responsePtr), false, "Failed to get connection for query parallelism performance.");
        return false;
    }
    SendResponse(std::move(responsePtr), true);
    return true;
}
}
