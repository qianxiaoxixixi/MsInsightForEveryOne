/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "StepHandler.h"

namespace Dic {
namespace Module {
namespace Summary {
using namespace Dic::Server;
void StepHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    PipelineStepRequest &request = dynamic_cast<PipelineStepRequest &>(*requestPtr.get());
    WsSession &session = *WsSessionManager::Instance().GetSession();
    std::unique_ptr<PipelineStepResponse> responsePtr = std::make_unique<PipelineStepResponse>();
    PipelineStepResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    SetResponseResult(response, true);
    auto database = Timeline::DataBaseManager::Instance().GetReadClusterDatabase();
    if (!database->GetStepIdList(response.body)) {
        SetResponseResult(response, false);
        ServerLog::Error("Failed to get step response data.");
    }
    session.OnResponse(std::move(responsePtr));
}

} // end of namespace Summary
} // end of namespace Module
} // end of namespace Dic