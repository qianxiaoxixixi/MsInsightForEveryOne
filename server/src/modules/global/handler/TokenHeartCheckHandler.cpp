/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "pch.h"
#include "WsSessionManager.h"
#include "TokenHeartCheckHandler.h"

namespace Dic {
namespace Module {
using namespace Dic::Server;

void TokenHeartCheckHandler::HandleRequest(std::unique_ptr<Request> requestPtr)
{
    auto &request = dynamic_cast<TokenHeartCheckRequest &>(*requestPtr.get());
    std::string sessionToken = request.token;
    if (!WsSessionManager::Instance().CheckSession(sessionToken)) {
        ServerLog::Error("Failed to check session, command = ", command);
        return;
    }
    WsSession &session = *WsSessionManager::Instance().GetSession(sessionToken);
    std::unique_ptr<TokenHeartCheckResponse> responsePtr = std::make_unique<TokenHeartCheckResponse>();
    TokenHeartCheckResponse &response = *responsePtr;
    SetBaseResponse(request, response);
    SetResponseResult(response, true);
    // add response to response queue in session
    session.OnResponse(std::move(responsePtr));
}
} // end of namespace Module
} // Dic