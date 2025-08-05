/*
* Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
#ifndef PROFILER_SERVER_QUERY_LEAKS_MEMORY_EVENT_HANDLER_H
#define PROFILER_SERVER_QUERY_LEAKS_MEMORY_EVENT_HANDLER_H

#include "MemoryDetailRequestHandler.h"

namespace Dic::Module::MemoryDetail {
class QueryLeaksMemoryEventHandler : public MemoryDetailRequestHandler {
public:
    QueryLeaksMemoryEventHandler() { command = Protocol::REQ_RES_LEAKS_MEMORY_EVENTS; }
    ~QueryLeaksMemoryEventHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
}  // namespace Dic::Module::MemoryDetail
#endif  // PROFILER_SERVER_QUERY_LEAKS_MEMORY_EVENT_HANDLER_H
