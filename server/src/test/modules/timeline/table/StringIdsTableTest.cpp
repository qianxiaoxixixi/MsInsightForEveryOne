/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include <string>
#include "StringIdsTable.h"
#include "TestCaseDatabaseUtil.h"
using namespace Dic::Protocol;
using namespace Dic::TimeLine::TestCaseUtil;
class StringIdsTableTest : public ::testing::Test {};

/**
 * 测试StringIdsTable字段映射
 */
TEST_F(StringIdsTableTest, testStringIdsTableColumnMaping)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE  STRING_IDS ("
        "  id INTEGER,"
        "  value TEXT,"
        "  PRIMARY KEY (id)"
        ");";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert = "INSERT INTO STRING_IDS (id, value) VALUES (1, 'aaaa'), (2, 'bbb');";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<StringIdsPO> stringIdsPOs;
    Dic::Protocol::StringIdsTable stringIdsTable;
    const uint64_t expectSize = 2;
    const uint64_t expectId = 2;
    const uint64_t index = 1;
    stringIdsTable.Select(StringIdsColumn::ID, StringIdsColumn::VALUE).ExcuteQuery(db, stringIdsPOs);
    EXPECT_EQ(stringIdsPOs.size(), expectSize);
    EXPECT_EQ(stringIdsPOs[index].value, "bbb");
    EXPECT_EQ(stringIdsPOs[index].id, expectId);
}
