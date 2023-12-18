/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_TOKENHEARTCHECKHANDLER_H
#define PROFILER_SERVER_TOKENHEARTCHECKHANDLER_H

#include "GlobalHandler.h"
#include "GlobalProtocolRequest.h"

namespace Dic {
namespace Module {
class TokenHeartCheckHandler : public GlobalHandler {
public:
    TokenHeartCheckHandler()
    {
        command = REQ_RES_TOKEN_DESTROY;
    }
    ~TokenHeartCheckHandler() override = default;

    void HandleRequest(std::unique_ptr<Request> requestPtr) override;
};
} // end of namespace Module
} // Dic

#endif // PROFILER_SERVER_TOKENHEARTCHECKHANDLER_H
