/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERY_MEMORY_STATIC_OPERATOR_LIST_HANDLER_H
#define PROFILER_SERVER_QUERY_MEMORY_STATIC_OPERATOR_LIST_HANDLER_H

#include "MemoryRequestHandler.h"

namespace Dic {
    namespace Module {
        namespace Memory {
            class QueryMemoryStaticOperatorListHandler : public MemoryRequestHandler {
            public:
                QueryMemoryStaticOperatorListHandler()
                {
                    command = Protocol::REQ_RES_MEMORY_STATIC_OP_MEMORY_LIST;
                };
                ~QueryMemoryStaticOperatorListHandler() override = default;
                void HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
            };
        } // end of namespace Memory
    } // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_QUERY_MEMORY_STATIC_OPERATOR_LIST_HANDLER_H
