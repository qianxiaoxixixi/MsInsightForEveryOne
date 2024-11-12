/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_ENUMAPITYPE_H
#define PROFILER_SERVER_ENUMAPITYPE_H
#include "TextTableColum.h"
#include "Table.h"
namespace Dic::Module::Timeline {
struct EnumApiTypePO {
    uint64_t id = 0;
    std::string name;
};
class EnumApiTypeTable : public Table<EnumApiTypePO> {
public:
    EnumApiTypeTable() = default;
    ~EnumApiTypeTable() override = default;
    std::unordered_map<uint64_t, std::string> QueryStrMap(const std::vector<uint64_t> &ids, const std::string &fileId);

protected:
    const std::unordered_map<std::string_view, assign> &GetAssignMap() override
    {
        static std::unordered_map<std::string_view, assign> assignMap = {
            { EnumApiTypeColumn::ID, SetId },
            { EnumApiTypeColumn::NAME, SetName },
        };
        return assignMap;
    }

    const std::string &GetTableName() override
    {
        static std::string tableName = "ENUM_API_TYPE";
        return tableName;
    }
    static void SetId(EnumApiTypePO &enumApiTypePO, const std::unique_ptr<SqliteResultSet> &resultSet);
    static void SetName(EnumApiTypePO &enumApiTypePO, const std::unique_ptr<SqliteResultSet> &resultSet);
};
}
#endif // PROFILER_SERVER_ENUMAPITYPE_H
