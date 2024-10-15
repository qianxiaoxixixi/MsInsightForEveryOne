/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "PytorchApiTable.h"
namespace Dic::Module::Timeline {
void PytorchApiTable::SetId(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.id = resultSet->GetUint64(PytorchApiColumn::ID);
}

void PytorchApiTable::SetTimestamp(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.timestamp = resultSet->GetUint64(PytorchApiColumn::TIMESTAMP);
}

void PytorchApiTable::SetEndTime(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.endTime = resultSet->GetUint64(PytorchApiColumn::ENDTIME);
}

void PytorchApiTable::SetGlobalTid(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.globalTid = resultSet->GetUint64(PytorchApiColumn::GLOBAL_TID);
}

void PytorchApiTable::SetConnectionId(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.connectionId = resultSet->GetUint64(PytorchApiColumn::CONNECTIONID);
}

void PytorchApiTable::SetName(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.name = resultSet->GetUint64(PytorchApiColumn::NAME);
}

void PytorchApiTable::SetSequenceNumber(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.sequenceNumber = resultSet->GetUint64(PytorchApiColumn::SEQUENCE_NUMBER);
}

void PytorchApiTable::SetFwdThreadId(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.fwdThreadId = resultSet->GetUint64(PytorchApiColumn::FWD_THREAD_ID);
}

void PytorchApiTable::SetInputDtypes(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.inputDtypes = resultSet->GetUint64(PytorchApiColumn::INPUT_DTYPES);
}

void PytorchApiTable::SetInputShapes(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.inputShapes = resultSet->GetUint64(PytorchApiColumn::INPUT_SHAPES);
}

void PytorchApiTable::SetCallchainId(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.callchainId = resultSet->GetUint64(PytorchApiColumn::CALL_CHAIN_ID);
}

void PytorchApiTable::SetType(PytorchApiPO &pytorchApiPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    pytorchApiPO.type = resultSet->GetUint64(PytorchApiColumn::TYPE);
}
}
