/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_REQUESTCONTEXT_H
#define PROFILER_SERVER_REQUESTCONTEXT_H
#include <memory>
#include <string>
#include <thread>
#include <iostream>

namespace Dic {
namespace Server {
class RequestContext {
public:
    // 获取当前线程的请求上下文
    static RequestContext& GetInstance()
    {
        static thread_local RequestContext instance;
        return instance;
    }

    void resetError()
    {
        error_ = {};
    }

    void SetError(Protocol::ErrorMessage error)
    {
        error_ = error;
    }

    // 获取响应信息
    Protocol::ErrorMessage GetError()
    {
        return error_;
    }

private:
    RequestContext() = default;
    ~RequestContext() = default;

    // 禁止拷贝和赋值
    RequestContext(const RequestContext&) = delete;
    RequestContext& operator=(const RequestContext&) = delete;

    Protocol::ErrorMessage error_;
};
}  // namespace Server
}  // namespace Dic

#endif  // PROFILER_SERVER_REQUESTCONTEXT_H
