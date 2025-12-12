/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "TrackInfoManager.h"
#include "TraceTime.h"
#include "QueryThreadsSameOperatorHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;
bool QueryThreadsSameOperatorHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    UnitThreadsOperatorsRequest &request = dynamic_cast<UnitThreadsOperatorsRequest &>(*requestPtr.get());
    WsSession &session = *WsSessionManager::Instance().GetSession();
    std::unique_ptr<UnitThreadsOperatorsResponse> responsePtr = std::make_unique<UnitThreadsOperatorsResponse>();
    UnitThreadsOperatorsResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    std::string warnMsg;
    if (!request.params.CheckParams(minTimestamp, warnMsg)) {
        ServerLog::Warn(warnMsg);
        SetTimelineError(ErrorCode::PARAMS_ERROR);
        SetResponseResult(response, false);
        session.OnResponse(std::move(responsePtr));
        return false;
    }
    auto db = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.rankId);
    if (db == nullptr) {
        ServerLog::Error("Query threads same operator failed to get connection.");
        SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
        SetResponseResult(response, false);
        session.OnResponse(std::move(responsePtr));
        return false;
    }
    std::vector<uint64_t> trackIdList;
    auto &TrackInfoManagerIns = TrackInfoManager::Instance();
    for (const auto& process : request.params.processes) {
        for (const auto& tid : process.tidList) {
            trackIdList.emplace_back(TrackInfoManagerIns.GetTrackId(request.params.rankId, process.pid, tid));
        }
    }
    bool result = db->QueryThreadSameOperatorsDetails(request.params, response.body, minTimestamp, trackIdList);
    if (!result) {
        SetTimelineError(ErrorCode::QUERY_THREAD_SAME_OPERATORS_DETAIL_FAILED);
    }
    SetResponseResult(response, result);
    // add response to response queue in session
    session.OnResponse(std::move(responsePtr));
    return result;
}

} // Timeline
} // Module
} // Dic