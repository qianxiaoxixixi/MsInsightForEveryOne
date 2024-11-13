/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "PythonApiRepo.h"
#include "TrackInfoManager.h"
#include "../../../DatabaseTestCaseMockUtil.cpp"
#include "TableDefaultMock.h"
using namespace Dic::Module::Timeline;
using namespace Dic::TimeLine::Table::Default::Mock;
using namespace Dic::Global::PROFILER::MockUtil;
class PythonApiRepoTest : public ::testing::Test {
protected:
    const std::string pythonApiSql =
        "CREATE TABLE PYTORCH_API (startNs TEXT, endNs TEXT, globalTid INTEGER, connectionId INTEGER, name INTEGER, "
        "sequenceNumber INTEGER, fwdThreadId INTEGER, inputDtypes INTEGER, inputShapes INTEGER, callchainId INTEGER, "
        "depth integer);";
    const std::string stringIdsSql = "CREATE TABLE STRING_IDS (id INTEGER PRIMARY KEY,value TEXT);";
    const std::string chainSql = "CREATE TABLE PYTORCH_CALLCHAINS (id INTEGER, stack INTEGER, stackDepth INTEGER);";
    void SetUp() override
    {
        TrackInfoManager::Instance().Reset();
    }

    void TearDown() override
    {
        TrackInfoManager::Instance().Reset();
    }

    void TestQuerySliceDetailInfoNormalPrepare(PytorchApiDependency &dependency)
    {
        sqlite3 *db = nullptr;
        DatabaseTestCaseMockUtil::OpenDB(db);
        DatabaseTestCaseMockUtil::CreateTable(db, pythonApiSql);
        DatabaseTestCaseMockUtil::CreateTable(db, stringIdsSql);
        DatabaseTestCaseMockUtil::CreateTable(db, chainSql);
        std::string pythonApiInsert =
            "INSERT INTO \"main\".\"PYTORCH_API\" (\"startNs\", \"endNs\", \"globalTid\", \"connectionId\", \"name\", "
            "\"sequenceNumber\", \"fwdThreadId\", \"inputDtypes\", \"inputShapes\", \"callchainId\", \"depth\") VALUES "
            "('1718180919237611490', '1718180919237618540', 8785587534252168, 820, 268435456, 1, 2, 3, 4, "
            "5, 4);";
        std::string chainInsert = "INSERT INTO \"main\".\"PYTORCH_CALLCHAINS\" (\"id\", \"stack\", \"stackDepth\") "
            "VALUES (5, 268436792, 0);\n"
            "INSERT INTO \"main\".\"PYTORCH_CALLCHAINS\" (\"id\", \"stack\", \"stackDepth\") VALUES (5, 268436793, 1);";
        std::string stringInsert =
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (268435456, 'qqq');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (3, 'aaa');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (4, 'nnn');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (268436792, 'bbb');\n"
            "INSERT INTO \"main\".\"STRING_IDS\" (\"id\", \"value\") VALUES (268436793, 'ggg');";
        DatabaseTestCaseMockUtil::InsertData(db, pythonApiInsert);
        DatabaseTestCaseMockUtil::InsertData(db, chainInsert);
        DatabaseTestCaseMockUtil::InsertData(db, stringInsert);
        dependency.stringIdsTableMock = std::make_unique<StringIdsTableMock>();
        dependency.stringIdsTableMock->SetDb(db);
        dependency.pytorchApiTableMock = std::make_unique<PytorchApiTableMock>();
        dependency.pytorchApiTableMock->SetDb(db);
        dependency.pytorchCallchainsTableMock = std::make_unique<PytorchCallchainsTableMock>();
        dependency.pytorchCallchainsTableMock->SetDb(db);
    }
};

/**
 * 测试根据id查询算子详情,正常情况
 */
TEST_F(PythonApiRepoTest, TestQuerySliceDetailInfoNormal)
{
    class PythonApiRepoRepoMock : public PythonApiRepo {
    public:
        void SetMock(PytorchApiDependency &dependency)
        {
            pytorchApiTable = std::move(dependency.pytorchApiTableMock);
            pytorchCallchainsTable = std::move(dependency.pytorchCallchainsTableMock);
            stringIdsTable = std::move(dependency.stringIdsTableMock);
        }
    };
    PytorchApiDependency dependency;
    TestQuerySliceDetailInfoNormalPrepare(dependency);
    PythonApiRepoRepoMock repo;
    repo.SetMock(dependency);
    SliceQuery query;
    query.sliceId = "1";
    query.rankId = "hhh";
    CompeteSliceDomain slice;
    bool result = repo.QuerySliceDetailInfo(query, slice);
    EXPECT_EQ(result, true);
    const uint64_t expectStart = 1718180919237611490;
    const uint64_t expectEnd = 1718180919237618540;
    EXPECT_EQ(slice.name, "qqq");
    EXPECT_EQ(slice.timestamp, expectStart);
    EXPECT_EQ(slice.endTime, expectEnd);
    const std::string expectArgs = "{\"sequenceNumber\":\"1\",\"fwdThreadId\":\"2\",\"connectionId\":\"820\","
        "\"inputShapes\":\"nnn\",\"inputDtypes\":\"aaa\",\"Call stack\":\"bbb;\\nggg;\\n\"}";
    EXPECT_EQ(slice.args, expectArgs);
}

/**
 * 测试根据id查询算子详情,算子不存在的情况
 */
TEST_F(PythonApiRepoTest, TestQuerySliceDetailInfoWhenSliceNotExistThenReturnFalse)
{
    PythonApiRepo repo;
    SliceQuery query;
    query.sliceId = "1";
    query.rankId = "hhh";
    CompeteSliceDomain slice;
    bool result = repo.QuerySliceDetailInfo(query, slice);
    EXPECT_EQ(result, false);
}