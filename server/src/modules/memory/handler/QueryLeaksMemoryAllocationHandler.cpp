/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "DataBaseManager.h"
#include "ProjectExplorerManager.h"
#include "QueryLeaksMemoryAllocationHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
bool QueryLeaksMemoryAllocationHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    auto &request = dynamic_cast<LeaksMemoryAllocationRequest &>(*requestPtr.get());
    std::unique_ptr<LeaksMemoryAllocationsResponse> responsePtr = std::make_unique<LeaksMemoryAllocationsResponse>();
    LeaksMemoryAllocationsResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    std::string errorMsg;
    if (!request.params.CommonCheck(errorMsg)) {
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }
    auto memoryDatabase = Timeline::DataBaseManager::Instance().GetLeaksMemoryDatabase("");
    if (memoryDatabase == nullptr) {
        errorMsg = "Get leaks memory database failed when querying allocations.";
        Server::ServerLog::Error(errorMsg);
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }
    std::vector<MemoryAllocation> allocations;
    memoryDatabase->QueryMemoryAllocations(request.params, allocations);
    if (allocations.empty()) {
        Server::ServerLog::Warn("Query memory allocations: empty data.");
        SendResponse(std::move(responsePtr), true);
        return true;
    }
    responsePtr->minTimestamp = allocations[0].timestamp;
    responsePtr->maxTimestamp = allocations.back().timestamp;
    responsePtr->allocations = std::move(allocations);
    SendResponse(std::move(responsePtr), true);
    return true;
}
} // Memory
} // Module
} // Dic