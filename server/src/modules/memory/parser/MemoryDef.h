/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_MEMORYDEF_H
#define PROFILER_SERVER_MEMORYDEF_H

#include <string>
#include <set>
#include "FileDef.h"

namespace Dic::Module::Memory {

struct MemoryFilePairs {
    std::set<std::string> operatorFiles;
    std::set<std::string> recordFiles;
};

struct Record {
    std::string component;
    int64_t timesTamp;
    double totalAllocated;
    double totalReserved;
    double totalActivated;
    std::string deviceType;
    std::string streamId;
};

struct Operator {
    std::string name;
    double size;
    int64_t allocationTime;
    int64_t releaseTime;
    double duration;
    int64_t activeReleaseTime;
    double activeDuration;
    double allocationAllocated;
    double allocationReserved;
    double allocationActive;
    double releaseAllocated;
    double releaseReserved;
    double releaseActive;
    std::string streamId;
    std::string deviceType;
};

// field in operator_memory
const std::string ALLOCATION_TIME = "Allocation Time(us)";
const std::string RELEASE_TIME = "Release Time(us)";
const std::string ACTIVE_RELEASE_TIME = "Active Release Time(us)";
const std::string ACTIVE_DURATION = "Active Duration(us)";
const std::string STREAM_PTR = "Stream Ptr";
const std::string ALLOCATION_ALLOCATED_MB = "Allocation Total Allocated(MB)";
const std::string ALLOCATION_RESERVED_MB = "Allocation Total Reserved(MB)";
const std::string ALLOCATION_ACTIVE_MB = "Allocation Total Active(MB)";
const std::string RELEASE_ALLOCATED_MB = "Release Total Allocated(MB)";
const std::string RELEASE_RESERVED_MB = "Release Total Reserved(MB)";
const std::string RELEASE_ACTIVE_MB = "Release Total Active(MB)";
const std::string ALLOCATION_ALLOCATED_KB = "Allocation Total Allocated(KB)";
const std::string ALLOCATION_RESERVED_KB = "Allocation Total Reserved(KB)";
const std::string ALLOCATION_ACTIVE_KB = "Allocation Total Active(KB)";
const std::string RELEASE_ALLOCATED_KB = "Release Total Allocated(KB)";
const std::string RELEASE_RESERVED_KB = "Release Total Reserved(KB)";
const std::string RELEASE_ACTIVE_KB = "Release Total Active(KB)";
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

// table header
const std::vector<std::string> MEMORY_RECORD_HEADER = {
    COMPONENT, TIMESTAMP, TOTAL_ALLOCATED_MB, TOTAL_RESERVED_MB, TOTAL_ACTIVE_MB, DEVICETYPE, STREAM_PTR
};
const std::vector<std::string> OPERATOR_MEMORY_HEADER = {
    NAME, SIZE, ALLOCATION_TIME, RELEASE_TIME, DURATION, ACTIVE_RELEASE_TIME, ACTIVE_DURATION,
    ALLOCATION_ALLOCATED_MB, ALLOCATION_RESERVED_MB, ALLOCATION_ACTIVE_MB,
    RELEASE_ALLOCATED_MB, RELEASE_RESERVED_MB, RELEASE_ACTIVE_MB,
    STREAM_PTR, DEVICETYPE
};
} // end of namespace Dic::Module::Memory

#endif // PROFILER_SERVER_MEMORYDEF_H
