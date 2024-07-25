/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "QueryOperatorSizeHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
using namespace Dic::Server;
void QueryOperatorSizeHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    MemoryOperatorSizeRequest &request = dynamic_cast<MemoryOperatorSizeRequest &>(*requestPtr.get());
    WsSession &session = *WsSessionManager::Instance().GetSession();
    std::unique_ptr<MemoryOperatorSizeResponse> responsePtr = std::make_unique<MemoryOperatorSizeResponse>();
    MemoryOperatorSizeResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    auto database = Timeline::DataBaseManager::Instance().GetMemoryDatabase(request.params.rankId);
    if (!database->QueryOperatorSize(response.size.minSize, response.size.maxSize, request.params.rankId)) {
        SetResponseResult(response, false);
        ServerLog::Error("Failed to query operator size data.");
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