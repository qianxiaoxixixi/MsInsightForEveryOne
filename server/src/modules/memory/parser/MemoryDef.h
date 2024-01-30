/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_MEMORYDEF_H
#define PROFILER_SERVER_MEMORYDEF_H

#include <string>
#include "FileDef.h"

namespace Dic::Module::Memory {

struct MemoryFilePair {
    std::string operatorFile;
    std::string recordFile;
};

struct Record {
    std::string component;
    int64_t timesTamp;
    double totalAllocated;
    double totalReserved;
    double totalActivated;
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

// field in operator_memory
const std::string ALLOCATION_TIME = "Allocation Time(us)";
const std::string RELEASE_TIME = "Release Time(us)";
const std::string ACTIVE_RELEASE_TIME = "Active Release Time(us)";
const std::string ACTIVE_DURATION = "Active Duration(us)";
const std::string STREAM_PTR = "Stream Ptr";
const std::vector<std::string> OPERATOR_CSV = {NAME, ALLOCATION_TIME, RELEASE_TIME, SIZE, DURATION};
const std::vector<std::string> OPERATOR_CSV_MSPROF = {NAME, ALLOCATION_TIME, SIZE, DURATION};

// field in memory_record
const std::string TOTAL_ALLOCATED_MB = "Total Allocated(MB)";
const std::string TOTAL_RESERVED_MB = "Total Reserved(MB)";
const std::string TOTAL_ACTIVE_MB = "Total Active(MB)";
const std::string TOTAL_ALLOCATED_KB = "Total Allocated(KB)";
const std::string TOTAL_RESERVED_KB = "Total Reserved(KB)";
const std::string TOTAL_ACTIVE_KB = "Total Active(KB)";
const std::vector<std::string> RECORD_CSV = {COMPONENT, TIMESTAMP, DEVICETYPE, TOTAL_ALLOCATED_MB, TOTAL_RESERVED_MB};
const std::vector<std::string> RECORD_CSV_MSPROF = {
    COMPONENT, TIMESTAMP, DEVICE, TOTAL_ALLOCATED_KB, TOTAL_RESERVED_KB
};
} // end of namespace Dic::Module::Memory

#endif // PROFILER_SERVER_MEMORYDEF_H
