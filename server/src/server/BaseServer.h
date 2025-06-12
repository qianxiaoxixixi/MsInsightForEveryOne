/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 */

#ifndef DATA_INSIGHT_CORE_BASE_SERVER_H
#define DATA_INSIGHT_CORE_BASE_SERVER_H

#include <string>
#include "WsSessionImpl.h"

namespace Dic {
namespace Server {
class BaseServer {
public:
    BaseServer(const std::string &host, int port) : host(host), port(port) {}
    virtual ~BaseServer() = default;
    virtual bool Start() = 0;
    virtual bool Stop() = 0;
    const std::string &GetHost() const
    {
        return host;
    }
    int GetPort() const
    {
        return port;
    };

protected:
    std::string host = "127.0.0.1";
    int port;
};
} // end of namespace Server
} // end of namespace Dic

#endif // DATA_INSIGHT_CORE_BASE_SERVER_H
