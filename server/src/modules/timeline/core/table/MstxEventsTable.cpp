/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "MstxEventsTable.h"
namespace Dic::Module::Timeline {
void MstxEventsTable::SetId(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.id = resultSet->GetUint64(MstxEventsColumn::ID);
}

void MstxEventsTable::SetTimestamp(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.timestamp = resultSet->GetUint64(MstxEventsColumn::TIMESTAMP);
}

void MstxEventsTable::SetEndTime(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.endTime = resultSet->GetUint64(MstxEventsColumn::ENDTIME);
}

void MstxEventsTable::SetEventType(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.eventType = resultSet->GetUint64(MstxEventsColumn::EVENT_TYPE);
}

void MstxEventsTable::SetRangeId(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.rangeId = resultSet->GetUint64(MstxEventsColumn::RANG_ID);
}

void MstxEventsTable::SetCategory(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.category = resultSet->GetUint64(MstxEventsColumn::CATEGORY);
}

void MstxEventsTable::SetMessage(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.message = resultSet->GetUint64(MstxEventsColumn::MESSAGE);
}

void MstxEventsTable::SetGlobalTid(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.globalTid = resultSet->GetUint64(MstxEventsColumn::GLOBAL_TID);
}

void MstxEventsTable::SetEndGlobalTid(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.endGlobalTid = resultSet->GetUint64(MstxEventsColumn::END_GLOBAL_TID);
}

void MstxEventsTable::SetConnectionId(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.connectionId = resultSet->GetUint64(MstxEventsColumn::CONNECTION_ID);
}

void MstxEventsTable::SetDomainId(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.domainId = resultSet->GetUint64(MstxEventsColumn::DOMAIN_ID);
}

void MstxEventsTable::SetDepth(MstxEventsPO &mstxEventsPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    mstxEventsPO.depth = resultSet->GetUint64(MstxEventsColumn::DEPTH);
}
}
