/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERY_MEMORY_STATIC_OPERATOR_SIZE_HANDLER_H
#define PROFILER_SERVER_QUERY_MEMORY_STATIC_OPERATOR_SIZE_HANDLER_H

#include "MemoryRequestHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
class QueryMemoryStaticOperatorSizeHandler : public MemoryRequestHandler {
public:
    QueryMemoryStaticOperatorSizeHandler()
    {
        command = Protocol::REQ_RES_MEMORY_STATIC_OP_MEMORY_MIN_MAX;
    };
    ~QueryMemoryStaticOperatorSizeHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
    bool GetRespectiveData(std::shared_ptr<VirtualMemoryDataBase> database,
                           StaticOperatorSize &compareData, StaticOperatorSize &baselineData,
                           MemoryStaticOperatorSizeRequest &request, std::string &errorMsg);
    void ExecuteComparisonAlgorithm(const StaticOperatorSize &compareData, const StaticOperatorSize &baselineData,
                                    MemoryStaticOperatorSizeResponse &response);
};

} // end of namespace Memory
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_QUERY_MEMORY_STATIC_OPERATOR_SIZE_HANDLER_H
