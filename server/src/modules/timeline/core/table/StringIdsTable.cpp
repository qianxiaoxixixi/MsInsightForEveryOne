/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "StringIdsTable.h"
namespace Dic::Module::Timeline {
void StringIdsTable::SetId(StringIdsPO &stringIdsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    stringIdsPO.id = resultSet->GetUint64(StringIdsColumn::ID);
}

void StringIdsTable::SetValue(StringIdsPO &stringIdsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    stringIdsPO.value = resultSet->GetString(StringIdsColumn::VALUE);
}
}
