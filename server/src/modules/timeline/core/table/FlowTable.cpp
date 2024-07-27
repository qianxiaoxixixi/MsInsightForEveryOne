/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "FlowTable.h"
namespace Dic {
namespace Module {
namespace Timeline {
    void FlowTable::IdHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.id = resultSet->GetUint64(FlowColumn::ID);
    }
    void FlowTable::FlowIdHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.flowId = resultSet->GetString(FlowColumn::FLOW_ID);
    }
    void FlowTable::NameHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.name = resultSet->GetString(FlowColumn::NAME);
    }
    void FlowTable::CatHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.cat = resultSet->GetString(FlowColumn::CAT);
    }
    void FlowTable::TrackIdHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.trackId = resultSet->GetUint64(FlowColumn::TRACK_ID);
    }
    void FlowTable::TimeStampHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.timestamp = resultSet->GetUint64(FlowColumn::TIMESTAMP);
    }
    void FlowTable::TypeHandle(FlowPO &flowPO, const std::unique_ptr<SqliteResultSet> &resultSet)
    {
        flowPO.type = resultSet->GetString(FlowColumn::TYPE);
    }
}
}
}
