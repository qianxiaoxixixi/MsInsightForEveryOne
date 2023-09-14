/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "QueryCommunicationDetailInfoHandler.h"
#include "ServerLog.h"
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "TraceTime.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;

bool QueryCommunicationDetailInfoHandler::GetResponseData(const Protocol::CommunicationDetailParams& params,
                                                          std::vector<Protocol::CommunicationDetail> &details,
                                                          int32_t &totalNum)
{
    std::string threadName = "Group 0 Communication";
    std::string notOverlap = "Communication(Not Overlapped)";
    auto database = Timeline::DataBaseManager::Instance().GetTraceDatabase(params.rankId);
    int64_t opTrackId = database->GetTrackIdList(threadName);
    int64_t notOverlapTrackId = database->GetTrackIdList(notOverlap);
    totalNum = database->QueryCommunicationTotalNum(std::to_string(opTrackId));

    if (!database->GetCommunicationDetails(opTrackId, details)) {
        ServerLog::Error("Failed to get communication detail.");
        return false;
    }

    for (Protocol::CommunicationDetail &detail: details) {
        double duration = detail.totalDuration;
        double timestamp = detail.startTime + static_cast<double>(Timeline::TraceTime::Instance().GetStartTime());
        double totalTime = 0;
        std::vector<double> res = database->QueryNotOverlapTime(notOverlapTrackId, timestamp, duration);
        for (double re: res) {
            totalTime += re;
        }
        detail.notOverlapDuration = totalTime;
        detail.overlapDuration = duration - totalTime;
    }
    return true;
}

void QueryCommunicationDetailInfoHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    CommunicationDetailRequest &request = dynamic_cast<CommunicationDetailRequest &>(*requestPtr.get());
    std::string token = request.token;
    if (!WsSessionManager::Instance().CheckSession(token)) {
        ServerLog::Warn("Failed to check session, token = ", StringUtil::AnonymousString(token),
                        ", command = ", command);
        return;
    }
    WsSession &session = *WsSessionManager::Instance().GetSession(token);
    std::unique_ptr<CommunicationDetailResponse> responsePtr = std::make_unique<CommunicationDetailResponse>();
    CommunicationDetailResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    auto database = Timeline::DataBaseManager::Instance().GetTraceDatabase(request.params.rankId);
    if (!GetResponseData(request.params, response.communication, response.totalNum)) {
        SetResponseResult(response, false);
        session.OnResponse(std::move(responsePtr));
        return;
    }
    SetResponseResult(response, true);
    // add response to response queue in session
    session.OnResponse(std::move(responsePtr));
}

} // Timeline
} // Module
} // Dic