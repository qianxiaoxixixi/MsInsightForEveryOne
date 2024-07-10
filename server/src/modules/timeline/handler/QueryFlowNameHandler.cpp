//
// * Copyright (c) Huawei Technologies Co., Ltd. 2012-2023. All rights reserved.
//

#include "WsSessionManager.h"
#include "TraceTime.h"
#include "QueryFlowNameHandler.h"
namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;
void QueryFlowNameHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    UnitFlowNameRequest &request = dynamic_cast<UnitFlowNameRequest &>(*requestPtr.get());
    std::string token = request.token;
    if (!WsSessionManager::Instance().CheckSession(token)) {
        ServerLog::Warn("Failed to check session, command = ", command);
        return;
    }
    WsSession &session = *WsSessionManager::Instance().GetSession(token);
    std::unique_ptr<UnitFlowNameResponse> responsePtr = std::make_unique<UnitFlowNameResponse>();
    UnitFlowNameResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    SetResponseResult(response, true);
    // add response to response queue in session
    session.OnResponse(std::move(responsePtr));
}

} // Timeline
} // Module
} // Dic
