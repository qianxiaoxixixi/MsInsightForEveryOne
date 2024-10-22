// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_WSSESSION_H
#define PROFILER_SERVER_WSSESSION_H
#include "ProtocolUtil.h"
namespace Dic::Server {
class WsSession {
public:
    enum class Status {
        INIT,
        STARTED,
        CLOSED
    };
    virtual void Start() = 0;
    virtual Status GetStatus() const = 0;
    virtual void OnResponse(std::unique_ptr<Protocol::Response> responsePtr) = 0;
    virtual void OnEvent(std::unique_ptr<Protocol::Event> eventPtr) = 0;
    virtual void SetStatus(Status sessionStatus) = 0;
    virtual void WaitForExit(int milliSeconds = 10000) = 0;
};
}
#endif // PROFILER_SERVER_WSSESSION_H
