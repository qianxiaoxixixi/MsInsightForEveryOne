/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "QueryOpStatisticInfoHandler.h"
#include "DataBaseManager.h"
#include "OperatorProtocolRequest.h"
#include "OperatorProtocolResponse.h"
#include "WsSessionManager.h"
#include "ServerLog.h"

namespace Dic::Module::Operator {
    using namespace Dic::Server;

    void QueryOpStatisticInfoHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        OperatorStatisticInfoRequest &request = dynamic_cast<OperatorStatisticInfoRequest &>(*requestPtr);
        std::string token = request.token;
        if (!WsSessionManager::Instance().CheckSession(token)) {
            ServerLog::Error("Failed to check session token of Statistic Info, command = ", command);
            return;
        }
        WsSession &session = *WsSessionManager::Instance().GetSession(token);
        std::unique_ptr<OperatorStatisticInfoResponse> responsePtr = std::make_unique<OperatorStatisticInfoResponse>();
        OperatorStatisticInfoResponse &response = *responsePtr;
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