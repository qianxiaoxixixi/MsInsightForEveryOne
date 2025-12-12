/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "TraceTime.h"
#include "QueryKernelDetailHandler.h"
namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;

bool QueryKernelDetailHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    KernelDetailsRequest &request = dynamic_cast<KernelDetailsRequest &>(*requestPtr.get());
    std::unique_ptr<KernelDetailsResponse> responsePtr = std::make_unique<KernelDetailsResponse>();
    KernelDetailsResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    WsSession &session = *WsSessionManager::Instance().GetSession();
    auto database = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.rankId);
    if (database == nullptr) {
        SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    std::string error;
    request.params.Check(minTimestamp, error);
    if (!std::empty(error)) {
        ServerLog::Warn(error);
        SetTimelineError(ErrorCode::PARAMS_ERROR);
        SendResponse(std::move(responsePtr), false);
        return false;
    }

    std::string deviceId = DataBaseManager::Instance().GetDeviceIdFromRankId(request.params.rankId);
    if (deviceId.empty()) {
        ServerLog::Error("Query kernel detail failed to get deviceId. ");
        SetTimelineError(ErrorCode::GET_DEVICE_ID_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    request.params.deviceId = deviceId;
    if (!database->QueryKernelDetailData(request.params, response.body, minTimestamp)) {
        ServerLog::Error("Failed to get kernel detail response data.");
        SetTimelineError(ErrorCode::QUERY_KERNEL_DETAIL_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    SendResponse(std::move(responsePtr), true);
    return true;
}
} // Timeline
} // Module
} // Dic