/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "ThreadTable.h"

namespace Dic {
namespace Module {
namespace Timeline {
void ThreadTable::TrackIdHandle(ThreadPO &threadPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    threadPO.trackId = resultSet->GetUint64(ThreadColumn::TRACK_ID);
}

void ThreadTable::TidHandle(ThreadPO &threadPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    threadPO.tid = resultSet->GetString(ThreadColumn::TID);
}

void ThreadTable::PidHandle(ThreadPO &threadPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    threadPO.pid = resultSet->GetString(ThreadColumn::PID);
}

void ThreadTable::ThreadNameHandle(ThreadPO &threadPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    threadPO.threadName = resultSet->GetString(ThreadColumn::THREAD_NAME);
}

void ThreadTable::ThreadSortIndexHandle(ThreadPO &threadPO, const std::unique_ptr<SqliteResultSet> &resultSet)
{
    threadPO.threadSortIndex = resultSet->GetUint64(ThreadColumn::THREAD_SORT_INDEX);
}
}
}
}
