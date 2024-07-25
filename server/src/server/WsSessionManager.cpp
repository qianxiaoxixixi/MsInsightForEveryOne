/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#include "WsSessionManager.h"

namespace Dic {
namespace Server {
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

void WsSessionManager::ClearSessions()
{
    RemoveSession();
}

void WsSessionManager::RemoveSession()
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    session.release();
}

WsSession *WsSessionManager::GetSession()
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    return session.get();
}

WsSession *WsSessionManager::GetSession(const WsChannel *channel)
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    return (session && session->GetChannel() == channel) ? session.get() : nullptr;
}

bool WsSessionManager::CheckSession()
{
    return session && session->GetStatus() != WsSession::Status::CLOSED;
}

void WsSessionManager::OnEventByMainSession(Protocol::Event &event)
{
    std::unique_lock<std::mutex> lock(sessionMutex);
    if (session) {
        session->SendEvent(event);
    }
}

} // end of namespace Server
} // end of namespace Dic