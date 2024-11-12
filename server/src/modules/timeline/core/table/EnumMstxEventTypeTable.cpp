/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "EnumMstxEventTypeTable.h"
namespace Dic::Module::Timeline {
void EnumMstxEventTypeTable::SetId(EnumMstxEventTypePO &enumMstxEventTypePO,
    const std::unique_ptr<SqliteResultSet> &resultSet)
{
    enumMstxEventTypePO.id = resultSet->GetUint64(EnumMstxEventTypeColumn::ID);
}

void EnumMstxEventTypeTable::SetName(EnumMstxEventTypePO &enumMstxEventTypePO,
    const std::unique_ptr<SqliteResultSet> &resultSet)
{
    enumMstxEventTypePO.name = resultSet->GetString(EnumMstxEventTypeColumn::NAME);
}

std::unordered_map<uint64_t, std::string> EnumMstxEventTypeTable::QueryStrMap(const std::vector<uint64_t> &ids,
    const std::string &fileId)
{
    std::unordered_map<uint64_t, std::string> res;
    std::vector<EnumMstxEventTypePO> enumMstxEventTypePO;
    Select(EnumMstxEventTypeColumn::ID, EnumMstxEventTypeColumn::NAME)
        .In(EnumMstxEventTypeColumn::ID, ids)
        .ExcuteQuery(fileId, enumMstxEventTypePO);
    for (const auto &item : enumMstxEventTypePO) {
        res[item.id] = item.name;
    }
    return res;
}
}