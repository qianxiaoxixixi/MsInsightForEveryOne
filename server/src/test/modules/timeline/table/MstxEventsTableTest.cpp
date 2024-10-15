/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include <string>
#include "MstxEventsTable.h"
#include "TestCaseDatabaseUtil.h"
using namespace Dic::Protocol;
using namespace Dic::TimeLine::TestCaseUtil;
class MstxEventsTableTest : public ::testing::Test {};

TEST_F(MstxEventsTableTest, testMstxEventsTableColumnMaping)
{
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE MSTX_EVENTS ("
        "startNs INTEGER,"
        "endNs INTEGER,"
        "eventType INTEGER,"
        "rangeId INTEGER,"
        "category INTEGER,"
        "message INTEGER,"
        "globalTid INTEGER,"
        "endGlobalTid INTEGER,"
        "domainId INTEGER,"
        "connectionId INTEGER,"
        "depth integer);";
    TestCaseDatabaseUtil::CreateDatabse(db, sql);
    std::string sqlInsert = "INSERT INTO MSTX_EVENTS (startNs, endNs,eventType, rangeId,category, message,globalTid, "
        "endGlobalTid,domainId, connectionId,depth) VALUES (1, 2,3,4,5,6,7,8,9,10,11);";
    TestCaseDatabaseUtil::InsertData(db, sqlInsert);
    std::vector<MstxEventsPO> mstxEventsPOs;
    Dic::Protocol::MstxEventsTable mstxEventsTable;
    const uint64_t expectSize = 1;
    uint64_t expectId = 1;
    uint64_t initInt = 1;
    const uint64_t index = 0;
    mstxEventsTable.Select(MstxEventsColumn::ID, MstxEventsColumn::TIMESTAMP)
        .Select(MstxEventsColumn::ENDTIME, MstxEventsColumn::EVENT_TYPE)
        .Select(MstxEventsColumn::RANG_ID, MstxEventsColumn::CATEGORY)
        .Select(MstxEventsColumn::MESSAGE, MstxEventsColumn::GLOBAL_TID)
        .Select(MstxEventsColumn::END_GLOBAL_TID, MstxEventsColumn::DOMAIN_ID)
        .Select(MstxEventsColumn::CONNECTION_ID, MstxEventsColumn::DEPTH)
        .ExcuteQuery(db, mstxEventsPOs);
    EXPECT_EQ(mstxEventsPOs.size(), expectSize);
    EXPECT_EQ(mstxEventsPOs[index].id, expectId);
    EXPECT_EQ(mstxEventsPOs[index].timestamp, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].endTime, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].eventType, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].rangeId, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].category, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].message, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].globalTid, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].endGlobalTid, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].domainId, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].connectionId, initInt++);
    EXPECT_EQ(mstxEventsPOs[index].depth, initInt++);
}
