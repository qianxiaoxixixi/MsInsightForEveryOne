/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "QueryFlowCategoryListHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Server;
bool QueryFlowCategoryListHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    FlowCategoryListRequest &request = dynamic_cast<FlowCategoryListRequest &>(*requestPtr.get());
    WsSession &session = *WsSessionManager::Instance().GetSession();
    std::unique_ptr<FlowCategoryListResponse> responsePtr = std::make_unique<FlowCategoryListResponse>();
    FlowCategoryListResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);
    auto database = DataBaseManager::Instance().GetTraceDatabaseByRankId(request.params.rankId);
    if (database == nullptr) {
        ServerLog::Error("Query flow category list failed to get connection. ");
        SetTimelineError(ErrorCode::CONNECT_DATABASE_FAILED);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    // 状态校验
    if (!database->CheckValueFromStatusInfoTable(CONNECTION_UNIT, FINISH_STATUS)) {
        SetTimelineError(ErrorCode::CATEGORY_PARSE_NOT_FINISH);
        SendResponse(std::move(responsePtr), false);
        return false;
    }
    bool result = database->QueryFlowCategoryList(response.body.category, request.params.rankId);
    if (!result) {
        SetTimelineError(ErrorCode::QUERY_FLOW_CATEGORY_FAILED);
    }
    SetResponseResult(response, result);
    session.OnResponse(std::move(responsePtr));
    return result;
}

} // Timeline
} // Module
} // Dic