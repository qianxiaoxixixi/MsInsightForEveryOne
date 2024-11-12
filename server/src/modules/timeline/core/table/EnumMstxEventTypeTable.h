/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_ENUMMSTXEVENTTYPETABLE_H
#define PROFILER_SERVER_ENUMMSTXEVENTTYPETABLE_H
#include "TextTableColum.h"
#include "Table.h"
namespace Dic::Module::Timeline {
struct EnumMstxEventTypePO {
    uint64_t id = 0;
    std::string name;
};
class EnumMstxEventTypeTable : public Table<EnumMstxEventTypePO> {
public:
    EnumMstxEventTypeTable() = default;
    ~EnumMstxEventTypeTable() override = default;
    std::unordered_map<uint64_t, std::string> QueryStrMap(const std::vector<uint64_t> &ids, const std::string &fileId);

protected:
    const std::unordered_map<std::string_view, assign> &GetAssignMap() override
    {
        static std::unordered_map<std::string_view, assign> assignMap = {
            { EnumMstxEventTypeColumn::ID, SetId },
            { EnumMstxEventTypeColumn::NAME, SetName },
        };
        return assignMap;
    }

    const std::string &GetTableName() override
    {
        static std::string tableName = "ENUM_MSTX_EVENT_TYPE";
        return tableName;
    }
    static void SetId(EnumMstxEventTypePO &enumMstxEventTypePO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetName(EnumMstxEventTypePO &enumMstxEventTypePO, const std::unique_ptr<SqliteResultSet> &resultSet);
};
}
#endif // PROFILER_SERVER_ENUMMSTXEVENTTYPETABLE_H
