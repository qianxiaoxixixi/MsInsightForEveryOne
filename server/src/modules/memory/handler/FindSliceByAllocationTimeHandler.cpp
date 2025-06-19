/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "TraceTime.h"
#include "ProjectExplorerManager.h"
#include "FindSliceByAllocationTimeHandler.h"

namespace Dic::Module::Memory {
using namespace Dic::Server;

bool FindSliceByAllocationTimeHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    MemoryFindSliceRequest &request = dynamic_cast<MemoryFindSliceRequest &>(*requestPtr.get());
    std::unique_ptr<MemoryFindSliceResponse> responsePtr = std::make_unique<MemoryFindSliceResponse>();
    MemoryFindSliceResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    std::string paramCheckErrMsg;
    if (!request.params.CommonCheck(paramCheckErrMsg)) {
        SendResponse(std::move(responsePtr), false, paramCheckErrMsg);
        ServerLog::Warn("Invalid paramn. ", paramCheckErrMsg);
        return false;
    }
    if (renderEngine == nullptr) {
        SendResponse(std::move(responsePtr), false, "Failed to find slice. timeline not exist!");
        ServerLog::Warn("Failed to find slice. timeline not exist!");
        return false;
    }
    OperatorDomain target = operatorMemoryService->ComputeAllocationTimeById(request.params.rankId, request.params.id);
    if (std::empty(target.metaType)) {
        SendResponse(std::move(responsePtr), false, "Failed to query memory operator");
        ServerLog::Warn("Failed to query memory operator!");
        return false;
    }
    Timeline::CompeteSliceDomain slice = renderEngine->FindSliceByTimePoint(request.params.rankId, request.params.name,
                                                                            target.allocationTime, target.metaType);
    if (std::empty(slice.tid) && std::empty(slice.pid)) {
        SendResponse(std::move(responsePtr), false, "Failed to find slice in timeline!");
        return false;
    }
    // multi device text data, need replace cardId with min deviceId
    auto projects = ProjectExplorerManager::Instance().QueryProjectExplorer(request.projectName, {});
    if (!projects.empty() && projects[0].projectType != static_cast<int>(ProjectTypeEnum::DB)) {
        auto deviceInfos = projects[0].GetDeviceInfos();
        std::sort(deviceInfos.begin(), deviceInfos.end(), [](auto &l, auto &r) {
            return l->deviceId < r->deviceId;
        });
        if (!deviceInfos.empty()) {
            slice.cardId = deviceInfos[0]->rankId;
        }
    }

    response.data.metaType = target.metaType;
    response.data.depth = slice.depth;
    response.data.processId = slice.pid;
    response.data.threadId = slice.tid;
    response.data.rankId = slice.cardId;
    response.data.startTime = slice.timestamp - Timeline::TraceTime::Instance().GetStartTime();
    response.data.id = std::to_string(slice.id);
    response.data.duration = slice.duration;
    SendResponse(std::move(responsePtr), true);
    return true;
}
}
