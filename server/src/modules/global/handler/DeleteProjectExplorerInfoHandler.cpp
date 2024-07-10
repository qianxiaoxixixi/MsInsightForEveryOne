/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "pch.h"
#include "WsSessionManager.h"
#include "ProjectExplorerManager.h"
#include "DeleteProjectExplorerInfoHandler.h"

namespace Dic {
namespace Module {
using namespace Dic::Server;
using namespace Global;

void DeleteProjectExplorerInfoHandler::HandleRequest(std::unique_ptr<Request> requestPtr)
{
    auto &request = dynamic_cast<ProjectExplorerInfoDeleteRequest &>(*requestPtr.get());
    std::string sessionToken = request.token;
    if (!WsSessionManager::Instance().CheckSession(sessionToken)) {
        ServerLog::Error("Failed to check session, command = ", command);
        return;
    }
    WsSession &session = *WsSessionManager::Instance().GetSession(sessionToken);
    std::unique_ptr<ProjectExplorerInfoDeleteResponse> responsePtr =
            std::make_unique<ProjectExplorerInfoDeleteResponse>();
    ProjectExplorerInfoDeleteResponse &response = *responsePtr;
    SetBaseResponse(request, response);
    bool res = ProjectExplorerManager::Instance()
            .DeleteProjectAndFilePath(request.params.projectName, request.params.dataPath);
    SetResponseResult(response, res);
    // add response to response queue in session
    session.OnResponse(std::move(responsePtr));
}
} // end of namespace Module
} // Dic
