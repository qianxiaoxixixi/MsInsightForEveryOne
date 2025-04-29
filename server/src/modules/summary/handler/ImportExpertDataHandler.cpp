/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "ImportExpertDataHandler.h"
#include "SummaryProtocolRequest.h"
#include "SummaryProtocolResponse.h"
#include "ExpertHotspotManager.h"
namespace Dic {
namespace Module {
namespace Summary {
bool ImportExpertDataHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    ImportExpertDataRequest &request = dynamic_cast<ImportExpertDataRequest &>(*requestPtr.get());
    std::unique_ptr<ImportExpertDataResponse> responsePtr = std::make_unique<ImportExpertDataResponse>();
    ImportExpertDataResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    std::string errorMsg;
    if (!request.params.CheckParams(errorMsg)) {
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }

    if (!ExpertHotspotManager::InitExpertHotspotData(request.params.filePath, request.params.version, errorMsg)) {
        SendResponse(std::move(responsePtr), false, errorMsg);
        return false;
    }
    SendResponse(std::move(responsePtr), true);
    return true;
}
}
}
}