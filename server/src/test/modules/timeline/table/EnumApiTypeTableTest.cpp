/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include <string>
#include "EnumApiTypeTable.h"
#include "TestCaseDatabaseUtil.h"
using namespace Dic::Protocol;
using namespace Dic::TimeLine::TestCaseUtil;
class EnumApiTypeTableTest : public ::testing::Test {};

TEST_F(EnumApiTypeTableTest, testEnumApiTypeColumnMaping)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE ENUM_API_TYPE (id INTEGER PRIMARY KEY,name TEXT);";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert =
            "INSERT INTO \"main\".\"ENUM_API_TYPE\" (\"id\", \"name\") VALUES (10000, 'node');";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<EnumApiTypePO> EnumApiTypePOs;
    Dic::Protocol::EnumApiTypeTable table;
    const uint64_t expectSize = 1;
    const uint64_t expectId = 10000;
    const uint64_t index = 0;
    table.Select(EnumApiTypeColumn::ID, EnumApiTypeColumn::NAME)
            .ExcuteQuery(db, EnumApiTypePOs);
    EXPECT_EQ(EnumApiTypePOs.size(), expectSize);
    EXPECT_EQ(EnumApiTypePOs[index].id, expectId);
    EXPECT_EQ(EnumApiTypePOs[index].name, "node");
}
