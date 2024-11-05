/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "CounterTable.h"
namespace Dic::Module::Timeline {
void CounterTable::SetId(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    counterPO.id = resultSet->GetUint64(CounterColumn::ID);
}

void CounterTable::SetName(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    counterPO.name = resultSet->GetString(CounterColumn::NAME);
}

void CounterTable::SetPid(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    counterPO.pid = resultSet->GetString(CounterColumn::PID);
}

void CounterTable::SetTimestamp(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    counterPO.timestamp = resultSet->GetUint64(CounterColumn::TIMESTAMP);
}

void CounterTable::SetCat(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    counterPO.cat = resultSet->GetString(CounterColumn::CAT);
}

void CounterTable::SetArgs(CounterPO &counterPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    counterPO.args = resultSet->GetString(CounterColumn::ARGS);
}
}