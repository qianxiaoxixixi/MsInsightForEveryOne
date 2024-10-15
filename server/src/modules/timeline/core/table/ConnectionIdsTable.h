/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_CONNECTIONIDSTABLE_H
#define PROFILER_SERVER_CONNECTIONIDSTABLE_H
#include "TextTableColum.h"
#include "Table.h"
namespace Dic::Module::Timeline {
struct ConnectionIdsPO {
    uint64_t id = 0;
    uint64_t connectionId = 0;
};
class ConnectionIdsTable : public Table<ConnectionIdsPO> {
public:
    ConnectionIdsTable() = default;
    ~ConnectionIdsTable() override = default;

protected:
    const std::unordered_map<std::string_view, assign> &GetAssignMap() override
    {
        static std::unordered_map<std::string_view, assign> assignMap = {
            { ConnectionIdsColumn::ID, SetId },
            { ConnectionIdsColumn::CONNECTIONID, SetConnectionId },
        };
        return assignMap;
    }

    const std::string &GetTableName() override
    {
        static std::string tableName = "CONNECTION_IDS";
        return tableName;
    }
    static void SetId(ConnectionIdsPO &connectionIdsPO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetConnectionId(ConnectionIdsPO &connectionIdsPO, const std::unique_ptr<SqliteResultSet> &resultSet);
};
}
#endif // PROFILER_SERVER_CONNECTIONIDSTABLE_H
