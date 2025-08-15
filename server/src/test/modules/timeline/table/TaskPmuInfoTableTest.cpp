/*
* Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include <string>
#include "TaskPmuInfoTable.h"
#include "TestCaseDatabaseUtil.h"

using namespace Dic::Protocol;
using namespace Dic::TimeLine::TestCaseUtil;
class TaskPmuInfoTableTest : public ::testing::Test {};

TEST_F(TaskPmuInfoTableTest, GetPmuInfo)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE TASK_PMU_INFO (globalTaskId INTEGER, name INTEGER, value NUMERIC)";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert =
        "INSERT INTO \"main\".\"TASK_PMU_INFO\" (\"globalTaskId\", \"name\", \"value\") VALUES (1, 2, 3);";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<TaskPmuInfoPO> pmuInfos;
    TaskPmuInfoTable pmuInfoTable;
    const uint64_t expectSize = 1;
    pmuInfoTable.Select(TaskPmuInfoColumn::GLOBAL_TASK_ID, TaskPmuInfoColumn::NAME_ID, TaskPmuInfoColumn::VALUE_ID)
        .ExcuteQuery(db, pmuInfos);
    EXPECT_EQ(pmuInfos.size(), expectSize);
    EXPECT_EQ(pmuInfos.at(0).globalTaskId, 1); // 1
    EXPECT_EQ(pmuInfos.at(0).name, 2); // 2
    EXPECT_EQ(pmuInfos.at(0).value, 3); // 3
}