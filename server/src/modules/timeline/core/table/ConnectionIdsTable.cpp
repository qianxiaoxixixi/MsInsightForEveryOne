/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "ConnectionIdsTable.h"
namespace Dic::Module::Timeline {
void ConnectionIdsTable::SetId(ConnectionIdsPO &connectionIdsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    connectionIdsPO.id = resultSet->GetUint64(ConnectionIdsColumn::ID);
}

void ConnectionIdsTable::SetConnectionId(ConnectionIdsPO &connectionIdsPO,
    const std::unique_ptr<SqliteResultSet> &resultSet)
{
    connectionIdsPO.connectionId = resultSet->GetUint64(ConnectionIdsColumn::CONNECTIONID);
}
}
