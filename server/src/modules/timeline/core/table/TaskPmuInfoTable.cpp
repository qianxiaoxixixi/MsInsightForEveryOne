/*
* Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "TaskPmuInfoTable.h"
namespace Dic::Module::Timeline {
void TaskPmuInfoTable::GlobalTaskIdHandle(TaskPmuInfoPO& taskPmuInfoPO, const std::unique_ptr<SqliteResultSet>& resultSet)
{
    taskPmuInfoPO.globalTaskId = resultSet->GetUint64(TaskPmuInfoColumn::GLOBAL_TASK_ID);
}

void TaskPmuInfoTable::NameHandle(TaskPmuInfoPO& taskPmuInfoPO, const std::unique_ptr<SqliteResultSet>& resultSet)
{
    taskPmuInfoPO.name = resultSet->GetUint64(TaskPmuInfoColumn::NAME_ID);
}

void TaskPmuInfoTable::ValueHandle(TaskPmuInfoPO& taskPmuInfoPO, const std::unique_ptr<SqliteResultSet>& resultSet)
{
    taskPmuInfoPO.value = resultSet->GetDouble(TaskPmuInfoColumn::VALUE_ID);
}
}
