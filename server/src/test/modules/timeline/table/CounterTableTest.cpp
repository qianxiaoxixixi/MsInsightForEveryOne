/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include <string>
#include "CounterTable.h"
#include "TestCaseDatabaseUtil.h"
using namespace Dic::Module::Timeline;
using namespace Dic::TimeLine::TestCaseUtil;
class CounterTableTest : public ::testing::Test {};

TEST_F(CounterTableTest, TestCounterTableMaping)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE counter (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pid TEXT,timestamp "
        "INTEGER, cat TEXT, args TEXT);";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert =
        "INSERT INTO \"main\".\"counter\" (\"id\", \"name\", \"pid\", \"timestamp\", \"cat\", \"args\") VALUES (949, "
        "'read_bandwidth', '207553184', 1726717577337544450, 'hhh', '{\"value\":\"0\",\"acc_id\":\"14\"}');";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<CounterPO> counterPOS;
    CounterTable counterTable;
    const uint64_t expectSize = 1;
    const std::string expectPid = "207553184";
    const uint64_t index = 0;
    const std::string expectName = "read_bandwidth";
    const std::string expectCat = "hhh";
    const std::string expectArgs = "{\"value\":\"0\",\"acc_id\":\"14\"}";
    const uint64_t expectId = 949;
    const uint64_t expectTime = 1726717577337544450;
    counterTable.Select(CounterColumn::PID, CounterColumn::ID)
        .Select(CounterColumn::NAME, CounterColumn::TIMESTAMP)
        .Select(CounterColumn::CAT, CounterColumn::ARGS)
        .ExcuteQuery(db, counterPOS);
    EXPECT_EQ(counterPOS.size(), expectSize);
    EXPECT_EQ(counterPOS[index].id, expectId);
    EXPECT_EQ(counterPOS[index].name, expectName);
    EXPECT_EQ(counterPOS[index].pid, expectPid);
    EXPECT_EQ(counterPOS[index].timestamp, expectTime);
    EXPECT_EQ(counterPOS[index].cat, expectCat);
    EXPECT_EQ(counterPOS[index].args, expectArgs);
}
