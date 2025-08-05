/*
* Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/

#include "DataBaseManager.h"
#include "QueryLeaksMemoryEventHandler.h"

namespace Dic::Module::MemoryDetail {
bool QueryLeaksMemoryEventHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    auto &request = dynamic_cast<LeaksMemoryEventRequest &>(*requestPtr);
    std::unique_ptr<LeaksMemoryEventResponse> responsePtr = std::make_unique<LeaksMemoryEventResponse>();
    auto &response = *responsePtr;
    SetBaseResponse(request, response);
    std::string errorMsg;
    if (!request.params.CommonCheck(errorMsg)) {
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }
    auto database = Timeline::DataBaseManager::Instance().GetLeaksMemoryDatabase("");
    if (database == nullptr) {
        errorMsg = "Get leaks memory database failed when querying events table.";
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }
    int64_t total = database->QueryEventsByRequestParams(request.params, responsePtr->events);
    if (total < 0) {
        errorMsg = "Failed to query events table using params: Prepare sql failed.";
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }
    responsePtr->total = total;
    SendResponse(std::move(responsePtr), true);
    return true;
}
}  // namespace Dic::Module::MemoryDetail