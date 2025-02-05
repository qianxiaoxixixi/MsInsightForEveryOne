/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERY_OPERATOR_SIZE_HANDLER_H
#define PROFILER_SERVER_QUERY_OPERATOR_SIZE_HANDLER_H

#include "MemoryRequestHandler.h"

namespace Dic {
namespace Module {
namespace Memory {
class QueryMemoryOperatorSizeHandler : public MemoryRequestHandler {
public:
    QueryMemoryOperatorSizeHandler()
    {
        command = Protocol::REQ_RES_MEMORY_OPERATOR_MIN_MAX;
    };
    ~QueryMemoryOperatorSizeHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
    bool GetRespectiveData(std::shared_ptr<VirtualMemoryDataBase> database,
                           OperatorSize &compareData, OperatorSize &baselineData,
                           MemoryOperatorSizeRequest &request, std::string &errorMsg);
    void ExecuteComparisonAlgorithm(const OperatorSize &compareData, const OperatorSize &baselineData,
                                    MemoryOperatorSizeResponse &response);
};

} // end of namespace Memory
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_QUERY_OPERATOR_SIZE_HANDLER_H
