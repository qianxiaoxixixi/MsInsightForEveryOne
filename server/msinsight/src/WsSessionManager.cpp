/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */
#include "WsSessionManager.h"
namespace Dic::Server {
WsSessionManager &WsSessionManager::Instance()
{
    static WsSessionManager instance;
    return instance;
}
void WsSessionManager::AddSession(std::unique_ptr<WsSession> newSession)
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    if (!session) {
        newSession->Start();
        session = std::move(newSession);
    }
}
void WsSessionManager::RemoveSession()
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    session.reset();
}
WsSession *WsSessionManager::GetSession()
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    return session.get();
}
bool WsSessionManager::CheckSession()
{
    return session && session->GetStatus() != WsSession::Status::CLOSED;
}
} // end of namespace Dic