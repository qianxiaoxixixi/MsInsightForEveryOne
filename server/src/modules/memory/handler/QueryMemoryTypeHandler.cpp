/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "MemoryProtocolRequest.h"
#include "MemoryProtocolRespose.h"
#include "QueryMemoryTypeHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
    using namespace Dic::Server;
    void QueryMemoryTypeHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        MemoryTypeRequest &request = dynamic_cast<MemoryTypeRequest &>(*requestPtr.get());
        WsSession &session = *WsSessionManager::Instance().GetSession();
        std::unique_ptr<MemoryTypeResponse> responsePtr = std::make_unique<MemoryTypeResponse>();
        MemoryTypeResponse &response = *responsePtr.get();
        SetBaseResponse(request, response);
        auto database = Timeline::DataBaseManager::Instance().GetMemoryDatabase(request.rankId);
        if (!database->QueryMemoryType(response.type, response.graphId)) {
            SetResponseResult(response, false);
            ServerLog::Error("Failed to query memory type data.");
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