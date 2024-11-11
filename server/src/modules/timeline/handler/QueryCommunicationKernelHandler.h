/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/

#ifndef PROFILER_SERVER_COMMUNICATIONKERNERHANDLER_H
#define PROFILER_SERVER_COMMUNICATIONKERNERHANDLER_H

#include "TimelineRequestHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
class QueryCommunicationKernelHandler : public TimelineRequestHandler {
public:
    QueryCommunicationKernelHandler()
    {
        command = Protocol::REQ_RES_COMMUNICATION_KERNEL_DETAIL;
    };

    ~QueryCommunicationKernelHandler() override = default;

    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
    std::string GetRealRankId(const std::string &rankId);
};
}
}
}
#endif // PROFILER_SERVER_COMMUNICATIONKERNERHANDLER_H
