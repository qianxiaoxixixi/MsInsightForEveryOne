/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERYLEAKSMEMORYBLOCKHANDLER_H
#define PROFILER_SERVER_QUERYLEAKSMEMORYBLOCKHANDLER_H

#include "MemoryRequestHandler.h"
#include "MemoryProtocolRequest.h"
#include "MemoryProtocolRespose.h"

namespace Dic {
namespace Module {
namespace Memory {
class QueryLeaksMemoryBlockHandler : public MemoryRequestHandler {
public:
    QueryLeaksMemoryBlockHandler()
    {
        command = Protocol::REQ_RES_LEAKS_MEMORY_BLOCKS;
    }
    ~QueryLeaksMemoryBlockHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
    static void BuildBlocksResponse(const std::vector<MemoryBlock> &blocks, LeaksMemoryBlocksResponse &response);
};
} // Memory
} // Module
} // Dic

#endif // PROFILER_SERVER_QUERYLEAKSMEMORYBLOCKHANDLER_H
