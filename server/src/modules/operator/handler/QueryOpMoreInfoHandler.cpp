/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "QueryOpMoreInfoHandler.h"
#include "DataBaseManager.h"
#include "OperatorProtocolRequest.h"
#include "OperatorProtocolResponse.h"
#include "WsSessionManager.h"
#include "ServerLog.h"

namespace Dic::Module::Operator {
    using namespace Dic::Server;

    void QueryOpMoreInfoHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        OperatorMoreInfoRequest &request = dynamic_cast<OperatorMoreInfoRequest &>(*requestPtr);
        std::string token = request.token;
        if (!WsSessionManager::Instance().CheckSession(token)) {
            ServerLog::Error("Failed to check session token of More Info, command = ", command);
            return;
        }
        WsSession &session = *WsSessionManager::Instance().GetSession(token);
        std::unique_ptr<OperatorMoreInfoResponse> responsePtr = std::make_unique<OperatorMoreInfoResponse>();
        OperatorMoreInfoResponse &response = *responsePtr;
        SetBaseResponse(request, response);
        auto database = Timeline::DataBaseManager::Instance().GetSummaryDatabase(request.params.rankId);
        if (true) {
            SetResponseResult(response, true);
            session.OnResponse(std::move(responsePtr));
            return;
        }
        SetResponseResult(response, true);
        session.OnResponse(std::move(responsePtr));
    }

}