/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_MEMORYPROTOCOLREQUEST_H
#define PROFILER_SERVER_MEMORYPROTOCOLREQUEST_H

#include <string>
#include <optional>
#include "ProtocolDefs.h"
#include "ProtocolMessage.h"

namespace Dic {
namespace Protocol {

struct MemoryOperatorParams {
    std::string rankId;
    std::string type;
    std::string searchName;
    int64_t minSize;
    int64_t maxSize;
    double startTime;
    double endTime;
    int64_t currentPage = 0;
    int64_t pageSize = 0;
    std::string orderBy;
    std::string order;
};

struct MemoryOperatorRequest : public Request {
    MemoryOperatorRequest() : Request(REQ_RES_MEMORY_OPERATOR) {};
    MemoryOperatorParams params;
};

const std::string MEMORY_OVERALL_GROUP = "Overall";
const std::string MEMORY_STREAM_GROUP = "Stream";

struct MemoryComponentParams {
    std::string rankId;
    std::string type; // Overall, Stream
};

struct MemoryViewRequest : public Request {
    MemoryViewRequest() : Request(REQ_RES_MEMORY_VIEW) {};
    MemoryComponentParams params;
};

struct MemoryOperatorSizeRequest : public Request {
    MemoryOperatorSizeRequest() : Request(REQ_RES_MEMORY_OPERATOR_MIN_MAX) {};
    MemoryComponentParams params;
};

} // end of namespace Protocol
} // end of namespace Dic

#endif // PROFILER_SERVER_MEMORYPROTOCOLREQUEST_H
