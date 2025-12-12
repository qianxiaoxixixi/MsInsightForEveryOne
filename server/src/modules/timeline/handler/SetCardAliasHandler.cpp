/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "SetCardAliasHandler.h"
#include "WsSessionManager.h"
#include "DataBaseManager.h"


namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;
bool SetCardAliasHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    SetCardAliasRequest &request = dynamic_cast<SetCardAliasRequest &>(*requestPtr.get());
    std::unique_ptr<SetCardAliasResponse> responsePtr = std::make_unique<SetCardAliasResponse>();
    SetCardAliasResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    SetResponseResult(response, true);
    std::string errMsg;
    WsSession &session = *WsSessionManager::Instance().GetSession();
    if (!request.params.CheckParams(request.params.cardAlias, errMsg)) {
        SetTimelineError(ErrorCode::PARAMS_ERROR);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    auto database = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.rankId);
    if (database == nullptr) {
        SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }

    if (!database->SetCardAlias(request.params, response.body)) {
        SetTimelineError(ErrorCode::SET_CARD_ALIAS_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    SetResponseResult(response, true);
    session.OnResponse(std::move(responsePtr));
    return true;
}

} // Timeline
} // Module
} // Dic