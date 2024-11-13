/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "HardWareRepo.h"
#include "TrackInfoManager.h"
#include "../../../DatabaseTestCaseMockUtil.cpp"
#include "TableDefaultMock.h"
using namespace Dic::Module::Timeline;
using namespace Dic::TimeLine::Table::Default::Mock;
using namespace Dic::Global::PROFILER::MockUtil;
class HardWareRepoTest : public ::testing::Test {
protected:
    const std::string stringIdsSql = "CREATE TABLE STRING_IDS (id INTEGER PRIMARY KEY,value TEXT);";
    const std::string taskSql = "CREATE TABLE TASK (startNs INTEGER,endNs INTEGER,deviceId INTEGER,connectionId "
        "INTEGER,globalTaskId INTEGER,globalPid INTEGER,taskType INTEGER,contextId "
        "INTEGER,streamId INTEGER,taskId INTEGER,modelId INTEGER, depth integer);";
    const std::string computeSql =
        "CREATE TABLE COMPUTE_TASK_INFO (name INTEGER,globalTaskId INTEGER PRIMARY KEY,blockDim INTEGER,mixBlockDim "
        "INTEGER,taskType INTEGER,opType INTEGER,inputFormats INTEGER,inputDataTypes INTEGER,inputShapes "
        "INTEGER,outputFormats INTEGER,outputDataTypes INTEGER,outputShapes INTEGER,attrInfo INTEGER, waitNs INTEGER);";
    void SetUp() override
    {
        TrackInfoManager::Instance().Reset();
    }

    void TearDown() override
    {
        TrackInfoManager::Instance().Reset();
    }

    void TestQuerySliceDetailInfoNormalPrepare(HardWareDependency &dependency)
    {
        sqlite3 *db = nullptr;
        DatabaseTestCaseMockUtil::OpenDB(db);
        DatabaseTestCaseMockUtil::CreateTable(db, taskSql);
        DatabaseTestCaseMockUtil::CreateTable(db, computeSql);
        DatabaseTestCaseMockUtil::CreateTable(db, stringIdsSql);
        std::string taskInsert =
            "INSERT INTO \"main\".\"TASK\" (\"startNs\", \"endNs\", \"deviceId\", \"connectionId\", \"globalTaskId\", "
            "\"globalPid\", \"taskType\", \"contextId\", \"streamId\", \"taskId\", \"modelId\", \"depth\") VALUES "
            "(1718180918997521124, 1718180918999870771, 0, 7422, 5, 2045554, 320, 4294967295, 16, 3731, 4294967295, "
            "0);";
        std::string computeInsert =
            "INSERT INTO \"main\".\"COMPUTE_TASK_INFO\" (\"name\", \"globalTaskId\", \"blockDim\", "
            "\"mixBlockDim\", \"taskType\", \"opType\", \"inputFormats\", \"inputDataTypes\", "
            "\"inputShapes\", \"outputFormats\", \"outputDataTypes\", \"outputShapes\", "
            "\"attrInfo\", \"waitNs\") VALUES (7, 5, 9, 0, 320, 8, 1, 2, 3, 4, 5, 6, 7, 5340);";
        std::string stringInsert = "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (320, 'qqq');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (1, 'aaa');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (2, 'bbb');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (3, 'ccc');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (4, 'ddd');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (5, 'eee');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (6, 'fff');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (7, 'ggg');";
        DatabaseTestCaseMockUtil::InsertData(db, taskInsert);
        DatabaseTestCaseMockUtil::InsertData(db, computeInsert);
        DatabaseTestCaseMockUtil::InsertData(db, stringInsert);
        dependency.stringIdsTableMock = std::make_unique<StringIdsTableMock>();
        dependency.stringIdsTableMock->SetDb(db);
        dependency.computeTaskInfoTableMock = std::make_unique<ComputeTaskInfoTableMock>();
        dependency.computeTaskInfoTableMock->SetDb(db);
        dependency.taskTableMock = std::make_unique<TaskTableMock>();
        dependency.taskTableMock->SetDb(db);
    }
};

/**
 * 测试根据id查询算子详情,正常情况
 */
TEST_F(HardWareRepoTest, TestQuerySliceDetailInfoNormal)
{
    class HardWareRepoMock : public HardWareRepo {
    public:
        void SetMock(HardWareDependency &dependency)
        {
            taskTable = std::move(dependency.taskTableMock);
            computeTaskInfoTable = std::move(dependency.computeTaskInfoTableMock);
            stringIdsTable = std::move(dependency.stringIdsTableMock);
        }
    };
    HardWareDependency dependency;
    TestQuerySliceDetailInfoNormalPrepare(dependency);
    HardWareRepoMock hardWareRepoMock;
    hardWareRepoMock.SetMock(dependency);
    SliceQuery query;
    query.sliceId = "1";
    query.rankId = "hhh";
    CompeteSliceDomain slice;
    bool result = hardWareRepoMock.QuerySliceDetailInfo(query, slice);
    EXPECT_EQ(result, true);
    const uint64_t expectStart = 1718180918997521124;
    const uint64_t expectEnd = 1718180918999870771;
    EXPECT_EQ(slice.timestamp, expectStart);
    EXPECT_EQ(slice.endTime, expectEnd);
    const std::string expectArgs = "{\"modelId\":\"4294967295\",\"taskType\":\"qqq\",\"streamId\":\"16\",\"taskId\":"
        "\"3731\",\"connectionId\":\"7422\"}";
    EXPECT_EQ(slice.args, expectArgs);
    EXPECT_EQ(slice.sliceShape.inputShapes, "ccc");
    EXPECT_EQ(slice.sliceShape.inputFormats, "aaa");
    EXPECT_EQ(slice.sliceShape.inputDataTypes, "bbb");
    EXPECT_EQ(slice.sliceShape.outputShapes, "fff");
    EXPECT_EQ(slice.sliceShape.outputFormats, "ddd");
    EXPECT_EQ(slice.sliceShape.outputDataTypes, "eee");
    EXPECT_EQ(slice.sliceShape.attrInfo, "ggg");
}

/**
 * 测试根据id查询算子详情,算子不存在的情况
 */
TEST_F(HardWareRepoTest, TestQuerySliceDetailInfoWhenSliceNotExistThenReturnFalse)
{
    HardWareRepo hardWareRepo;
    SliceQuery query;
    query.sliceId = "1";
    query.rankId = "hhh";
    CompeteSliceDomain slice;
    bool result = hardWareRepo.QuerySliceDetailInfo(query, slice);
    EXPECT_EQ(result, false);
}