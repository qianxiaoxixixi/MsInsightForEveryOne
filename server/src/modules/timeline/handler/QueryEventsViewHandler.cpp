/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "TraceTime.h"
#include "QueryEventsViewHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;

bool QueryEventsViewHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    EventsViewRequest &request = dynamic_cast<EventsViewRequest &>(*requestPtr.get());

    std::unique_ptr<EventsViewResponse> responsePtr = std::make_unique<EventsViewResponse>();
    EventsViewResponse &response = *responsePtr.get();
    WsSession &session = *WsSessionManager::Instance().GetSession();
    SetBaseResponse(request, response);
    uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    std::string warnMsg;
    if (!request.params.CheckParams(minTimestamp, warnMsg)) {
        ServerLog::Warn(warnMsg);
        SetTimelineError(ErrorCode::PARAMS_ERROR);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    auto database = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.rankId);
    if (database == nullptr) {
        ServerLog::Error("Query events view failed to get connection.");
        SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    if (!request.params.tid.empty()) {
        request.params.threadIdList.emplace_back(request.params.tid);
    }
    if (!database->QueryEventsViewData(request.params, responsePtr->body, minTimestamp)) {
        ServerLog::Warn("Failed to get events view table response data.");
        SetTimelineError(ErrorCode::QUERY_EVENTS_VIEW_DATA_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    SendResponse(std::move(responsePtr), true);
    return true;
}
} // end of namespace Timeline
} // end of namespace Module
} // end of namespace Dic