/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "DataBaseManager.h"
#include "OperatorProtocolRequest.h"
#include "OperatorProtocolResponse.h"
#include "WsSessionManager.h"
#include "ServerLog.h"
#include "QueryOpComputeUnitHandler.h"

namespace Dic::Module::Operator {
    using namespace Dic::Server;

    void QueryOpComputeUnitHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        OperatorComputeUnitInfoRequest &request = dynamic_cast<OperatorComputeUnitInfoRequest &>(*requestPtr);
        std::string token = request.token;
        if (!WsSessionManager::Instance().CheckSession(token)) {
            ServerLog::Error("Failed to check session token of Compute Unit Info, command = ", command);
            return;
        }
        WsSession &session = *WsSessionManager::Instance().GetSession(token);
        auto responsePtr = std::make_unique<OperatorComputeUnitInfoResponse>();
        OperatorComputeUnitInfoResponse &response = *responsePtr;
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