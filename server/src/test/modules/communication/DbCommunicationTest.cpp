/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "MemoryProtocolRequest.h"
#include "DataBaseManager.h"
#include "DbClusterDataBase.h"
#include "ParamsParser.h"
#include "FileUtil.h"
#include "TraceTime.h"

using namespace Dic::Module::Timeline;
using namespace Dic::Module::FullDb;
using namespace Dic;

class DbCommunicationTest : public ::testing::Test {
public:
    static void SetUpTestSuite()
    {
        std::string currPath = Dic::FileUtil::GetCurrPath();
        const ParamsOption &option = ParamsParser::Instance().GetOption();
        ServerLog::Initialize(option.logPath, option.logSize, option.logLevel, to_string(option.wsPort));
        int index = currPath.find_last_of("server");
        currPath = currPath.substr(0, index + 1);
        std::string dbPath3 = R"(/src/test/test_data/full_db/)";
        DataBaseManager::Instance().SetDataType(DataType::DB);
        DataBaseManager::Instance().SetFileType(FileType::MS_PROF);
        auto culsterDatabase = DataBaseManager::Instance().CreateClusterDatabase(COMPARE, DataType::DB);
        culsterDatabase->OpenDb(currPath + dbPath3 + "cluster_analysis.db", false);
    }
    static void TearDownTestSuite() {}
};

TEST_F(DbCommunicationTest, QueryIterationsData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    std::vector<Dic::Protocol::IterationsOrRanksObject> responseBody;
    database->QueryIterations(responseBody);
    int expectSize = 2;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].iterationOrRankId, "1");
}

TEST_F(DbCommunicationTest, QueryOperatorNameData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::OperatorNamesParams requestParams;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.rankList = {};
    std::vector<Dic::Protocol::OperatorNamesObject> responseBody;
    database->QueryOperatorNames(requestParams, responseBody);
    int expectSize = 1325;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].operatorName, "Total Op Info");
    EXPECT_EQ(responseBody[1].operatorName, "hcom_allGather__018_3_1");
}

TEST_F(DbCommunicationTest, QueryOperatorNameDataWithRank)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::OperatorNamesParams requestParams;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.rankList = {"0"};
    std::vector<Dic::Protocol::OperatorNamesObject> responseBody;
    database->QueryOperatorNames(requestParams, responseBody);
    int expectSize = 1325;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].operatorName, "Total Op Info");
    EXPECT_EQ(responseBody[1].operatorName, "hcom_allGather__018_3_1");
}

TEST_F(DbCommunicationTest, QueryRanksData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::RanksParams requestParam;
    requestParam.dbIndex = "0";
    requestParam.iterationId = "2";
    std::vector<Dic::Protocol::IterationsOrRanksObject> responseBody;
    database->QueryRanksHandler(responseBody);
    int expectSize = 8;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].iterationOrRankId, "0");
    EXPECT_EQ(responseBody[1].iterationOrRankId, "1");
}

TEST_F(DbCommunicationTest, QueryDurationData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::DurationListParams requestParams;
    Protocol::DurationListsResponseBody responseBody;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.operatorName = "Total Op Info";
    database->QueryDurationList(requestParams, responseBody);
    int expectSize = 8;
    EXPECT_EQ(responseBody.durationList.size(), expectSize);
}

TEST_F(DbCommunicationTest, QueryDurationDataWithRank)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::DurationListParams requestParams;
    Protocol::DurationListsResponseBody responseBody;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.operatorName = "Total Op Info";
    requestParams.rankList = {"0"};
    database->QueryDurationList(requestParams, responseBody);
    int expectSize = 1;
    EXPECT_EQ(responseBody.durationList.size(), expectSize);
}

TEST_F(DbCommunicationTest, QueryBandwidthDistributionData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::DistributionDataParam requestParams;
    Dic::Protocol::DistributionResBody responseBody;
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.operatorName = "hcom_allGather__018_3_1";
    requestParams.transportType = "HCCS";
    requestParams.rankId = "1";
    database->QueryDistributionData(requestParams, responseBody);
    std::string expectResult = "{\"0.0001\":[7,0.0100401953125]}";
    EXPECT_EQ(responseBody.distributionData, expectResult);
}

TEST_F(DbCommunicationTest, QueryBandwidthData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::BandwidthDataParam requestParams;
    Dic::Protocol::BandwidthDataResBody responseBody;
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.operatorName = "Total Op Info";
    requestParams.rankId = "1";
    database->QueryBandwidthData(requestParams, responseBody);
    int expectSize = 2;
    EXPECT_EQ(responseBody.items.size(), expectSize);
    EXPECT_EQ(responseBody.items[0].transportType, "HCCS");
}

TEST_F(DbCommunicationTest, QueryIterationAndCommunicationGroupDBSuccess)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::CommunicationKernelParams requestParams;
    requestParams.name = "hcom_broadcast__293_0_1";
    requestParams.rankId = "0";
    Dic::Protocol::CommunicationKernelBody responseBody;
    database->QueryIterationAndCommunicationGroup(requestParams, responseBody);
    EXPECT_EQ(responseBody.step, "1");
    EXPECT_EQ(responseBody.group, "(0,1,2,3,4,5,6,7)");
}

TEST_F(DbCommunicationTest, QueryBandwidthDataWithErrorParamReturnExpectSize0)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::BandwidthDataParam requestParams;
    Dic::Protocol::BandwidthDataResBody responseBody;
    requestParams.iterationId = "2";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.operatorName = "Total Op Info";
    requestParams.rankId = "1";
    database->QueryBandwidthData(requestParams, responseBody);
    const int expectSize = 2;
    EXPECT_EQ(responseBody.items.size(), expectSize);
}

TEST_F(DbCommunicationTest, QueryOperatorsCount)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::OperatorDetailsParam requestParams;
    Dic::Protocol::OperatorDetailsResBody responseBody;
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.rankId = "1";
    database->QueryOperatorsCount(requestParams, responseBody);
    const int expectSize = 1324;
    EXPECT_EQ(responseBody.count, expectSize);
    requestParams.iterationId = "2";
    requestParams.stage = "(0, 1, 2, 3, 4, 5, 6, 7)";
    requestParams.rankId = "0";
    database->QueryOperatorsCount(requestParams, responseBody);
    const int stageExpectSize = 0;
    EXPECT_EQ(responseBody.count, stageExpectSize);
}

TEST_F(DbCommunicationTest, GetCommunicationGroups)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::MatrixGroupParam requestParams;
    Dic::Protocol::MatrixGroupResponseBody responseBody;
    requestParams.iterationId = "1";
    database->GetGroups(requestParams, responseBody);
    int expectSize = 1;
    EXPECT_EQ(responseBody.groupList.size(), expectSize);
    EXPECT_EQ(responseBody.groupList[0], "(0,1,2,3,4,5,6,7)");
}

TEST_F(DbCommunicationTest, QueryMatrixData)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::MatrixBandwidthParam requestParams;
    Dic::Protocol::MatrixListResponseBody responseBody;
    requestParams.iterationId = "1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.operatorName = "allgather-bottom1";
    database->QueryMatrixList(requestParams, responseBody);
    int expectSize = 64;
    EXPECT_EQ(responseBody.matrixList.size(), expectSize);
}

TEST_F(DbCommunicationTest, QueryAllCommunicationOperatorsDetails)
{
    auto database = DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    Dic::Protocol::OperatorDetailsParam requestParams;
    Dic::Protocol::OperatorDetailsResBody responseBody;
    requestParams.iterationId = "step1";
    requestParams.stage = "(0,1,2,3,4,5,6,7)";
    requestParams.rankId = "0";
    requestParams.currentPage = 1;
    requestParams.pageSize = 100; // pageSize = 100
    database->QueryAllOperators(requestParams, responseBody);
    int expectSize = 100;
    EXPECT_EQ(responseBody.allOperators.size(), expectSize);
    EXPECT_EQ(responseBody.allOperators[0].operatorName, "hcom_allReduce__293_647_1");
}