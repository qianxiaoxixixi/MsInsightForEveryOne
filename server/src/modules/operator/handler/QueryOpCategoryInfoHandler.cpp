/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "DataBaseManager.h"
#include "OperatorProtocolRequest.h"
#include "OperatorProtocolResponse.h"
#include "WsSessionManager.h"
#include "ServerLog.h"
#include "QueryOpCategoryInfoHandler.h"

namespace Dic::Module::Operator {
    using namespace Dic::Server;

    void QueryOpCategoryInfoHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        OperatorCategoryInfoRequest &request = dynamic_cast<OperatorCategoryInfoRequest &>(*requestPtr);
        std::string token = request.token;
        if (!WsSessionManager::Instance().CheckSession(token)) {
            ServerLog::Error("Failed to check session token of Category Info, command = ", command);
            return;
        }
        WsSession &session = *WsSessionManager::Instance().GetSession(token);
        std::unique_ptr<OperatorCategoryInfoResponse> responsePtr = std::make_unique<OperatorCategoryInfoResponse>();
        OperatorCategoryInfoResponse &response = *responsePtr;
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