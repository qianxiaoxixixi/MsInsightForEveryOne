/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_MEMORYDEF_H
#define PROFILER_SERVER_MEMORYDEF_H

#include <string>

namespace Dic {
namespace Module {
namespace Memory {

struct MemoryFilePair {
    std::string operatorFile;
    std::string recordFile;
};

struct Record {
    std::string component;
    int64_t timesTamp;
    double totalAllocated;
    double totalReserved;
    std::string deviceType;
};

struct Operator {
    std::string name;
    double size;
    int64_t allocationTime;
    int64_t releaseTime;
    double duration;
    std::string deviceType;
};

} // end of namespace Memory
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_MEMORYDEF_H
