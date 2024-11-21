/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 */

#ifndef DATA_INSIGHT_CORE_SERVER_SESSION_MANAGER_H
#define DATA_INSIGHT_CORE_SERVER_SESSION_MANAGER_H

#include <memory>
#include <string>
#include <mutex>
#include <optional>
#include "WsSession.h"

namespace Dic {
namespace Server {
class WsSessionManager {
public:
    static WsSessionManager &Instance();

    void AddSession(std::unique_ptr<WsSession> newSession);
    void RemoveSession();
    WsSession *GetSession();
    bool CheckSession();

private:
    WsSessionManager() = default;
    ~WsSessionManager() = default;

    std::mutex sessionMutex;
    std::unique_ptr<WsSession> session = nullptr;
};
} // end of namespace Server
} // end of namespace Dic

#endif // DATA_INSIGHT_CORE_SERVER_SESSION_MANAGER_H
