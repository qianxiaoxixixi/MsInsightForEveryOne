/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "TraceTime.h"
#include "QueryThreadTracesSummaryHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;
bool QueryThreadTracesSummaryHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    UnitThreadTracesSummaryRequest &request = dynamic_cast<UnitThreadTracesSummaryRequest &>(*requestPtr.get());
    WsSession &session = *WsSessionManager::Instance().GetSession();
    std::unique_ptr<UnitThreadTracesSummaryResponse> responsePtr = std::make_unique<UnitThreadTracesSummaryResponse>();
    UnitThreadTracesSummaryResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    std::string warnMsg;
    if (!request.params.CheckParams(minTimestamp, warnMsg)) {
        ServerLog::Warn(warnMsg);
        SetTimelineError(ErrorCode::PARAMS_ERROR);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    auto database = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.cardId);
    if (database == nullptr) {
        ServerLog::Error("Query thread traces summary failed to get connection.");
        SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    bool result = database->QueryThreadTracesSummary(request.params, response.body,
                                                     minTimestamp);
    if (!result) {
        SetTimelineError(ErrorCode::QUERY_THREAD_TRACES_SUMMARY_FAILED);
        SetResponseResult(response, result);
    }
    SendResponse(std::move(responsePtr), result);
    return result;
}

} // Timeline
} // Module
} // Dic