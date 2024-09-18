// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.

#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "MemoryProtocolRequest.h"
#include "MemoryProtocolRespose.h"
#include "QueryMemoryResourceTypeHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
    using namespace Dic::Server;
    void QueryMemoryResourceTypeHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        MemoryResourceTypeRequest &request = dynamic_cast<MemoryResourceTypeRequest &>(*requestPtr.get());
        WsSession &session = *WsSessionManager::Instance().GetSession();
        std::unique_ptr<MemoryResourceTypeResponse> responsePtr = std::make_unique<MemoryResourceTypeResponse>();
        MemoryResourceTypeResponse &response = *responsePtr.get();
        SetBaseResponse(request, response);
        std::string errorMsg;
        if (!CheckStrParamValid(request.rankId, errorMsg)) {
            SetResponseResult(response, false);
            ServerLog::Error(errorMsg);
            session.OnResponse(std::move(responsePtr));
            return;
        }
        auto database = Timeline::DataBaseManager::Instance().GetMemoryDatabase(request.rankId);
        if (!database->QueryMemoryResourceType(response.type)) {
            SetResponseResult(response, false);
            ServerLog::Error("Failed to query memory resource type data.");
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