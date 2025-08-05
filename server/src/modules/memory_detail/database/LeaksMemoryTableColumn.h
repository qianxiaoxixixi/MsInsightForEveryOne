/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_LEAKSMEMORYCOLUMN_H
#define PROFILER_SERVER_LEAKSMEMORYCOLUMN_H

#include "pch.h"

namespace Dic::Module::MemoryDetail {
struct SqliteDbTableColumn {
    std::string_view name;
    std::string_view key;
    bool visible{true};
    bool sortable{false};
    bool searchable{false};

    SqliteDbTableColumn(std::string_view name, std::string_view key, bool visible, bool sortable, bool searchable)
        : name(name),
          key(key),
          visible(visible),
          sortable(sortable),
          searchable(searchable) {}

    SqliteDbTableColumn(std::string_view name, std::string_view key)
        : name(name),
          key(key)
    {
        visible = false;
    }
};

inline std::vector<SqliteDbTableColumn>::const_iterator FindColumnByKey(std::string_view key,
                                                                        const std::vector<SqliteDbTableColumn> &columns)
{
    return std::find_if(columns.begin(), columns.end(), [key](const SqliteDbTableColumn& col) {
            return key == col.key;
    });
}

namespace MemoryEventTableColumn {
    constexpr std::string_view ID = "ID";
    constexpr std::string_view EVENT = "Event";
    constexpr std::string_view EVENT_TYPE = "`Event Type`";
    constexpr std::string_view NAME = "Name";
    constexpr std::string_view TIMESTAMP = "`Timestamp(ns)`";
    constexpr std::string_view PROCESS_ID = "`Process Id`";
    constexpr std::string_view THREAD_ID = "`Thread Id`";
    constexpr std::string_view DEVICE_ID = "`Device Id`";
    constexpr std::string_view PTR = "Ptr";
    constexpr std::string_view ATTR = "Attr";
    inline const std::vector<SqliteDbTableColumn> FIELD_FULL_COLUMNS = {
        {ID, "id", true, true, false},
        {EVENT, "event", true, true, true},
        {EVENT_TYPE, "eventType", true, true, true},
        {NAME, "name", true, true, true},
        {TIMESTAMP, "timestamp", true, true, true},
        {PROCESS_ID, "processId", true, true, true},
        {THREAD_ID, "threadId", true, true, true},
        {DEVICE_ID, "deviceId"},
        {PTR, "ptr", true, true, true},
        {ATTR, "attr", true, true, true}
    };
}
namespace MemoryAllocationTableColumn {
    constexpr std::string_view ID = "id";
    constexpr std::string_view TIMESTAMP = "timestamp";
    constexpr std::string_view TOTAL_SIZE = "totalSize";
    constexpr std::string_view OPTIMIZED = "optimized";
    constexpr std::string_view DEVICE_ID = "deviceId";
    constexpr std::string_view EVENT_TYPE = "eventType";
    constexpr std::string_view FULL_COLUMNS_WITHOUT_ID[] = {TIMESTAMP, TOTAL_SIZE, OPTIMIZED,
                                                            DEVICE_ID, EVENT_TYPE};
    constexpr std::string_view FULL_COLUMNS[] = {ID, TIMESTAMP, TOTAL_SIZE,
                                                 OPTIMIZED, DEVICE_ID, EVENT_TYPE};
}
namespace MemoryBlockTableColumn {
    constexpr std::string_view ID = "id";
    constexpr std::string_view DEVICE_ID = "deviceId";
    constexpr std::string_view ADDR = "addr";
    constexpr std::string_view SIZE = "size";
    constexpr std::string_view START_TIMESTAMP = "startTimestamp";
    constexpr std::string_view END_TIMESTAMP = "endTimestamp";
    constexpr std::string_view EVENT_TYPE = "eventType";
    constexpr std::string_view OWNER = "owner";
    constexpr std::string_view ATTR = "attr";
    constexpr std::string_view PROCESS_ID = "processId";
    constexpr std::string_view THREAD_ID = "threadId";
    constexpr std::string_view FULL_COLUMNS_WITHOUT_ID[] = {DEVICE_ID, ADDR, SIZE, START_TIMESTAMP,
                                                            END_TIMESTAMP, EVENT_TYPE, OWNER,
                                                            ATTR, PROCESS_ID, THREAD_ID};
    inline const std::vector<SqliteDbTableColumn>  FIELD_FULL_COLUMNS = {
        {ID, "id", true, true, false},
        {DEVICE_ID, "deviceId"},
        {ADDR, "addr", true, true, true},
        {SIZE, "size", true, true, false},
        {START_TIMESTAMP, "startTimestamp", true, true, false},
        {END_TIMESTAMP, "endTimestamp", true, true, false},
        {EVENT_TYPE, "eventType", true, true, true},
        {OWNER, "owner", true, true, true},
        {PROCESS_ID, "processId", true, true, true},
        {THREAD_ID, "threadId", true, true, true},
        {ATTR, "attr", true, true, true}
    };
}
namespace MemoryPythonTraceTableColumn {
    constexpr std::string_view FUNC_INFO = "FuncInfo";
    constexpr std::string_view START_TIME = "`StartTime(ns)`";
    constexpr std::string_view END_TIME = "`EndTime(ns)`";
    constexpr std::string_view THREAD_ID = "`Thread Id`";
    constexpr std::string_view PROCESS_ID = "`Process Id`";
}
}
#endif // PROFILER_SERVER_LEAKSMEMORYCOLUMN_H
