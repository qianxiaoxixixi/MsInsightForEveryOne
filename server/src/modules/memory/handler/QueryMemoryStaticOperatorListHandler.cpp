/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "MemoryProtocolRequest.h"
#include "MemoryProtocolRespose.h"
#include "QueryMemoryStaticOperatorListHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
    using namespace Dic::Server;
    void QueryMemoryStaticOperatorListHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        MemoryStaticOperatorListRequest &request =
                dynamic_cast<MemoryStaticOperatorListRequest &>(*requestPtr.get());
        WsSession &session = *WsSessionManager::Instance().GetSession();
        std::unique_ptr<MemoryStaticOperatorListResponse> responsePtr =
                std::make_unique<MemoryStaticOperatorListResponse>();
        MemoryStaticOperatorListResponse &response = *responsePtr.get();
        SetBaseResponse(request, response);
        auto database = Timeline::DataBaseManager::Instance().GetMemoryDatabase(request.params.rankId);
        if (!database->QueryStaticOperatorList(request.params, response.columnAttr, response.operatorDetails) or
            !database->QueryStaticOperatorsTotalNum(request.params, response.totalNum)) {
            SetResponseResult(response, false);
            ServerLog::Error("Failed to query memory static operator data.");
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