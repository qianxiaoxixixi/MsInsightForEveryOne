/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#ifndef PROFILER_SERVER_COUNTERTABLE_H
#define PROFILER_SERVER_COUNTERTABLE_H
#include "TextTableColum.h"
#include "Table.h"
namespace Dic::Module::Timeline {
struct CounterPO {
    uint64_t id = 0;
    std::string name;
    std::string pid;
    uint64_t timestamp = 0;
    std::string cat;
    std::string args;
};

class CounterTable : public Table<CounterPO> {
public:
    CounterTable() = default;
    ~CounterTable() override = default;

protected:
    const std::unordered_map<std::string_view, assign> &GetAssignMap() override
    {
        static std::unordered_map<std::string_view, assign> assignMap = {
            { CounterColumn::ID, SetId },   { CounterColumn::NAME, SetName },
            { CounterColumn::PID, SetPid }, { CounterColumn::TIMESTAMP, SetTimestamp },
            { CounterColumn::CAT, SetCat }, { CounterColumn::ARGS, SetArgs },
        };
        return assignMap;
    }

    const std::string &GetTableName() override
    {
        static std::string tableName = "counter";
        return tableName;
    }
    static void SetId(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetName(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetPid(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetTimestamp(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetCat(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetArgs(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet);
};
}
#endif // PROFILER_SERVER_COUNTERTABLE_H
