/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include <string>
#include "EnumMstxEventTypeTable.h"
#include "TestCaseDatabaseUtil.h"
using namespace Dic::Protocol;
using namespace Dic::TimeLine::TestCaseUtil;
class EnumMstxEventTypeTableTest : public ::testing::Test {};

TEST_F(EnumMstxEventTypeTableTest, testEnumMstxEventTypeColumnMaping)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE ENUM_MSTX_EVENT_TYPE (id INTEGER PRIMARY KEY,name TEXT);";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert =
            "INSERT INTO \"main\".\"ENUM_MSTX_EVENT_TYPE\" (\"id\", \"name\") VALUES (3, 'marker_ex');";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<EnumMstxEventTypePO> EnumMstxEventTypePOs;
    Dic::Protocol::EnumMstxEventTypeTable table;
    const uint64_t expectSize = 1;
    const uint64_t expectId = 3;
    const uint64_t index = 0;
    table.Select(EnumMstxEventTypeColumn::ID, EnumMstxEventTypeColumn::NAME)
            .ExcuteQuery(db, EnumMstxEventTypePOs);
    EXPECT_EQ(EnumMstxEventTypePOs.size(), expectSize);
    EXPECT_EQ(EnumMstxEventTypePOs[index].id, expectId);
    EXPECT_EQ(EnumMstxEventTypePOs[index].name, "marker_ex");
}
