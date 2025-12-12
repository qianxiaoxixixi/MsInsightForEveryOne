/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
*/
#include "QueryOneKernelHandler.h"
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "ProjectExplorerManager.h"
#include "TraceTime.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;

bool QueryOneKernelHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    KernelRequest &request = dynamic_cast<KernelRequest &>(*requestPtr.get());
    std::unique_ptr<OneKernelResponse> responsePtr = std::make_unique<OneKernelResponse>();
    OneKernelResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    std::string warnMsg;
    if (!request.params.CheckParams(minTimestamp, warnMsg)) {
        SetTimelineError(ErrorCode::PARAMS_ERROR);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    auto database = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.rankId);
    if (database == nullptr) {
        database = Timeline::DataBaseManager::Instance().GetTraceDatabaseWithOutHost(request.params.rankId);
        if (database == nullptr) {
            SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
            SendResponse(std::move(responsePtr), false);
            return false;
        }
    }
    if (!database->QueryKernelDepthAndThread(request.params, response.body, TraceTime::Instance().GetStartTime())) {
        SetTimelineError(ErrorCode::QUERY_KERNEL_DEPTH_AND_THREAD_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }

    SendResponse(std::move(responsePtr), true);
    return true;
}

} // Timeline
} // Module
} // Dic