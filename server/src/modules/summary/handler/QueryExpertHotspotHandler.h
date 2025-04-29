/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERYEXPERTHOTSPOTHANDLER_H
#define PROFILER_SERVER_QUERYEXPERTHOTSPOTHANDLER_H

#include "SummaryRequestHandler.h"
namespace Dic {
namespace Module {
namespace Summary {
class QueryExpertHotspotHandler : public SummaryRequestHandler {
public:
    QueryExpertHotspotHandler()
    {
        command = Protocol::REQ_RES_QUERY_EXPERT_HOTSPOT;
    }
    ~QueryExpertHotspotHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
}
}
}
#endif // PROFILER_SERVER_QUERYEXPERTHOTSPOTHANDLER_H
