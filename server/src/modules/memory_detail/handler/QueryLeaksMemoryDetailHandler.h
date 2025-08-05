/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERYLEAKSMEMORYDETAILHANDLER_H
#define PROFILER_SERVER_QUERYLEAKSMEMORYDETAILHANDLER_H

#include "MemoryDetailRequestHandler.h"

namespace Dic::Module::MemoryDetail {
class QueryLeaksMemoryDetailHandler : public MemoryDetailRequestHandler {
public:
    QueryLeaksMemoryDetailHandler() { command = Protocol::REQ_RES_LEAKS_MEMORY_DETAILS; }
    ~QueryLeaksMemoryDetailHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
}  // namespace Dic::Module::MemoryDetail
#endif  // PROFILER_SERVER_QUERYLEAKSMEMORYDETAILHANDLER_H
