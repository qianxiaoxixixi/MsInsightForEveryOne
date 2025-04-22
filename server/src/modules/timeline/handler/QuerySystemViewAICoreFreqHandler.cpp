/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "QuerySystemViewAICoreFreqHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;
bool QuerySystemViewAICoreFreqHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    SystemViewAICoreFreqRequest &request = dynamic_cast<SystemViewAICoreFreqRequest &>(*requestPtr.get());

    std::unique_ptr<SystemViewAICoreFreqResponse> responsePtr = std::make_unique<SystemViewAICoreFreqResponse>();
    WsSession &session = *WsSessionManager::Instance().GetSession();
    SystemViewAICoreFreqResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    std::string warnMsg;
    if (!request.params.CheckParams(warnMsg)) {
        ServerLog::Warn(warnMsg);
        SetResponseResult(response, false, warnMsg);
        session.OnResponse(std::move(responsePtr));
        return false;
    }
    SetResponseResult(response, true);
    auto database = DataBaseManager::Instance().GetTraceDatabase(request.params.rankId);
    if (database == nullptr) {
        ServerLog::Error("Query system view AI core freq failed to get connection.");
        session.OnResponse(std::move(responsePtr));
        return false;
    }
    if (!database->QuerySystemViewAICoreFreqData(request.params, response.body)) {
        SetResponseResult(response, false);
        ServerLog::Error("Failed to get system view AI core freq table response data.");
    }
    // add response to response queue in session
    session.OnResponse(std::move(responsePtr));
    return true;
}

} // Timeline
} // Module
} // Dic