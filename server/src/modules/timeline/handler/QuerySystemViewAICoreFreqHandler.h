/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/

#ifndef PROFILER_SERVER_QUERYSYSTEMVIEWAICOREFREQHANDLER_H
#define PROFILER_SERVER_QUERYSYSTEMVIEWAICOREFREQHANDLER_H

#include "TimelineRequestHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
class QuerySystemViewAICoreFreqHandler : public TimelineRequestHandler {
public:
    QuerySystemViewAICoreFreqHandler()
    {
        command = Protocol::REQ_RES_SYSTEM_VIEW_AICORE_FREQ ;
    };

    ~QuerySystemViewAICoreFreqHandler() override = default;

    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
} // end of namespace Timeline
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_QUERYSYSTEMVIEWAICOREFREQHANDLER_H
