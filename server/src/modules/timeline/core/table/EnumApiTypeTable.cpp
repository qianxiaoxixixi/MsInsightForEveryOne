/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "EnumApiTypeTable.h"
namespace Dic::Module::Timeline {
void EnumApiTypeTable::SetId(EnumApiTypePO &enumApiTypePO,
    const std::unique_ptr<SqliteResultSet> &resultSet)
{
    enumApiTypePO.id = resultSet->GetUint64(EnumApiTypeColumn::ID);
}

void EnumApiTypeTable::SetName(EnumApiTypePO &enumApiTypePO,
    const std::unique_ptr<SqliteResultSet> &resultSet)
{
    enumApiTypePO.name = resultSet->GetString(EnumApiTypeColumn::NAME);
}

std::unordered_map<uint64_t, std::string> EnumApiTypeTable::QueryStrMap(const std::vector<uint64_t> &ids,
    const std::string &fileId)
{
    std::unordered_map<uint64_t, std::string> res;
    std::vector<EnumApiTypePO> enumApiTypePO;
    Select(EnumApiTypeColumn::ID, EnumApiTypeColumn::NAME)
        .In(EnumApiTypeColumn::ID, ids)
        .ExcuteQuery(fileId, enumApiTypePO);
    for (const auto &item : enumApiTypePO) {
        res[item.id] = item.name;
    }
    return res;
}
}