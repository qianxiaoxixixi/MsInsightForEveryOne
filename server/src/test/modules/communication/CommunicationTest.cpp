/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "CommunicationProtocolRequest.h"
#include "DataBaseManager.h"
#include "../../TestSuit.cpp"

class CommunicationTest : TestSuit {
};

TEST_F(TestSuit, QueryIterationsData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetClusterDatabase();
    std::vector<Dic::Protocol::IterationsOrRanksObject> responseBody;
    database->QueryIterations(responseBody);
    int expectSize = 1;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].iterationOrRankId, "2");
}

TEST_F(TestSuit, QueryOperatorNameData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetClusterDatabase();
    Dic::Protocol::OperatorNamesParams requestParams;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "2";
    requestParams.stage = "p2p";
    std::vector<Dic::Protocol::OperatorNamesObject> responseBody;
    database->QueryOperatorNames(requestParams, responseBody);
    int expectSize = 2;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].operatorName, "Total Op Info");
    EXPECT_EQ(responseBody[1].operatorName, "hcom_send__822_0");
}

TEST_F(TestSuit, QueryOperatorNameDataWithRank)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetClusterDatabase();
    Dic::Protocol::OperatorNamesParams requestParams;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "2";
    requestParams.stage = "p2p";
    requestParams.rankList = {"0"};
    std::vector<Dic::Protocol::OperatorNamesObject> responseBody;
    database->QueryOperatorNames(requestParams, responseBody);
    int expectSize = 2;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].operatorName, "Total Op Info");
    EXPECT_EQ(responseBody[1].operatorName, "hcom_send__822_0");
}

TEST_F(TestSuit, QueryRanksData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetClusterDatabase();
    Dic::Protocol::RanksParams requestParam;
    requestParam.dbIndex = "0";
    requestParam.iterationId = "2";
    std::vector<Dic::Protocol::IterationsOrRanksObject> responseBody;
    database->QueryRanksHandler(requestParam, responseBody);
    int expectSize = 2;
    EXPECT_EQ(responseBody.size(), expectSize);
    EXPECT_EQ(responseBody[0].iterationOrRankId, "0");
    EXPECT_EQ(responseBody[1].iterationOrRankId, "1");
}

TEST_F(TestSuit, QueryDurationData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetClusterDatabase();
    Dic::Protocol::DurationListParams requestParams;
    std::vector<Dic::Protocol::Duration> responseBody;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "2";
    requestParams.stage = "p2p";
    requestParams.operatorName = "hcom_send__822_0";
    database->QueryDurationList(requestParams, responseBody);
    int expectSize = 2;
    EXPECT_EQ(responseBody.size(), expectSize);
}

TEST_F(TestSuit, QueryDurationDataWithRank)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetClusterDatabase();
    Dic::Protocol::DurationListParams requestParams;
    std::vector<Dic::Protocol::Duration> responseBody;
    requestParams.dbIndex = "0";
    requestParams.iterationId = "2";
    requestParams.stage = "p2p";
    requestParams.operatorName = "hcom_send__822_0";
    requestParams.rankList = {"0"};
    database->QueryDurationList(requestParams, responseBody);
    int expectSize = 1;
    EXPECT_EQ(responseBody.size(), expectSize);
}