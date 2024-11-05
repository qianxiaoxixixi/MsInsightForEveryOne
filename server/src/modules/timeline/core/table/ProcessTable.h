/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_PROCESSTABLE_H
#define PROFILER_SERVER_PROCESSTABLE_H
#include "TextTableColum.h"
#include "Table.h"
namespace Dic::Module::Timeline {
struct ProcessPO {
    std::string pid;
    std::string processName;
    std::string label;
    uint64_t processSortIndex;
};

class ProcessTable : public Table<ProcessPO> {
public:
    ProcessTable() = default;
    ~ProcessTable() override = default;

protected:
    const std::unordered_map<std::string_view, assign> &GetAssignMap() override
    {
        static std::unordered_map<std::string_view, assign> assignMap = {
            { ProcessColumn::PID, SetPid },
            { ProcessColumn::PROCESS_NAME, SetProcessName },
            { ProcessColumn::LABEL, SetLabel },
            { ProcessColumn::PROCESS_SORT_INDEX, SetProcessSortIndex },
        };
        return assignMap;
    }

    const std::string &GetTableName() override
    {
        static std::string tableName = "process";
        return tableName;
    }
    static void SetPid(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetProcessName(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetLabel(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetProcessSortIndex(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet);
};
}
#endif // PROFILER_SERVER_PROCESSTABLE_H
