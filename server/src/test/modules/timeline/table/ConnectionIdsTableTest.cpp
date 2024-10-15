/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include <string>
#include "ConnectionIdsTable.h"
#include "TestCaseDatabaseUtil.h"
using namespace Dic::Protocol;
using namespace Dic::TimeLine::TestCaseUtil;
class ConnectionIdsTableTest : public ::testing::Test {};

TEST_F(ConnectionIdsTableTest, testConnectionIdsTableColumnMaping)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE  CONNECTION_IDS ("
        "  id INTEGER,"
        "  connectionId INTEGER"
        ");";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert = "INSERT INTO CONNECTION_IDS (id, connectionId) VALUES (1, 67786), (2, 7487378);";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<ConnectionIdsPO> connectionIdsPOs;
    Dic::Protocol::ConnectionIdsTable connectionIdsTable;
    const uint64_t expectSize = 2;
    const uint64_t expectId = 2;
    const uint64_t expectConnectionId = 7487378;
    const uint64_t index = 1;
    connectionIdsTable.Select(ConnectionIdsColumn::ID, ConnectionIdsColumn::CONNECTIONID)
        .ExcuteQuery(db, connectionIdsPOs);
    EXPECT_EQ(connectionIdsPOs.size(), expectSize);
    EXPECT_EQ(connectionIdsPOs[index].connectionId, expectConnectionId);
    EXPECT_EQ(connectionIdsPOs[index].id, expectId);
}