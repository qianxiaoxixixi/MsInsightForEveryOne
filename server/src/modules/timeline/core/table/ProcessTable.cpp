/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "ProcessTable.h"
namespace Dic::Module::Timeline {
void ProcessTable::SetPid(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    processPO.pid = resultSet->GetString(ProcessColumn::PID);
}

void ProcessTable::SetProcessName(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    processPO.processName = resultSet->GetString(ProcessColumn::PROCESS_NAME);
}

void ProcessTable::SetLabel(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    processPO.label = resultSet->GetString(ProcessColumn::LABEL);
}

void ProcessTable::SetProcessSortIndex(ProcessPO &processPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    processPO.processSortIndex = resultSet->GetUint64(ProcessColumn::PROCESS_SORT_INDEX);
}
}
