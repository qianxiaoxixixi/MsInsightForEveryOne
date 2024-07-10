/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "MemoryProtocolRequest.h"
#include "MemoryProtocolRespose.h"
#include "QueryMemoryStaticOperatorGraphHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
    using namespace Dic::Server;
    void QueryMemoryStaticOperatorGraphHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        MemoryStaticOperatorGraphRequest &request =
                dynamic_cast<MemoryStaticOperatorGraphRequest &>(*requestPtr.get());
        std::string token = request.token;
        if (!WsSessionManager::Instance().CheckSession(token)) {
            ServerLog::Error("Failed to check session token , command = ", command);
            return;
        }
        WsSession &session = *WsSessionManager::Instance().GetSession(token);
        std::unique_ptr<MemoryStaticOperatorGraphResponse> responsePtr =
                std::make_unique<MemoryStaticOperatorGraphResponse>();
        MemoryStaticOperatorGraphResponse &response = *responsePtr.get();
        SetBaseResponse(request, response);
        auto database = Timeline::DataBaseManager::Instance().GetMemoryDatabase(request.params.rankId);
        if (!database->QueryStaticOperatorGraph(request.params, response.data)) {
            SetResponseResult(response, false);
            ServerLog::Error("Failed to query memory static operator graph data.");
            session.OnResponse(std::move(responsePtr));
            return;
        }
        SetResponseResult(response, true);
        // add response to response queue in session
        session.OnResponse(std::move(responsePtr));
    }
} // end of namespace Memory
} // end of namespace Module
} // end of namespace Dic