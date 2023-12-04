/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "MemoryProtocolRequest.h"
#include "DataBaseManager.h"
#include "../../TestSuit.cpp"

class MemoryTest : TestSuit {
};

TEST_F(TestSuit, QueryMemoryOperatorData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryOperatorParams requestParams;
    requestParams.rankId = "0";
    requestParams.currentPage = 0;
    requestParams.pageSize = 10; // page size = 10
    requestParams.startTime = -1;
    requestParams.endTime = -1;
    requestParams.minSize = -1;
    requestParams.maxSize = -1;
    std::vector<Dic::Protocol::MemoryOperator> responseBody;
    database->QueryOperatorDetail(requestParams, responseBody);
    int expectSize = 10;
    EXPECT_EQ(responseBody.size(), expectSize);
}

TEST_F(TestSuit, QueryMemoryOperatorWithTime)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryOperatorParams requestParams;
    requestParams.rankId = "0";
    requestParams.currentPage = 0;
    requestParams.pageSize = 100; // page size = 100
    requestParams.startTime = 0;
    requestParams.endTime = 1695120000000; // end time = 1695120000000
    requestParams.minSize = -1;
    requestParams.maxSize = -1;
    std::vector<Dic::Protocol::MemoryOperator> responseBody;
    database->QueryOperatorDetail(requestParams, responseBody);
    int expectSize = 28;
    EXPECT_EQ(responseBody.size(), expectSize);
}

TEST_F(TestSuit, QueryMemoryOperatorWithSize)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryOperatorParams requestParams;
    requestParams.rankId = "0";
    requestParams.currentPage = 0;
    requestParams.pageSize = 10; // page size = 10
    requestParams.startTime = -1;
    requestParams.endTime = -1;
    requestParams.minSize = 10; // min size = 10
    requestParams.maxSize = 100; // min size = 100
    std::vector<Dic::Protocol::MemoryOperator> responseBody;
    database->QueryOperatorDetail(requestParams, responseBody);
    int expectSize = 10;
    EXPECT_EQ(responseBody.size(), expectSize);
}

TEST_F(TestSuit, QueryOperatorsTotalNum)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryOperatorParams requestParams;
    requestParams.rankId = "0";
    requestParams.orderName = "cann::Graph_";
    requestParams.minSize = -1;
    requestParams.maxSize = -1;
    requestParams.startTime = -1;
    requestParams.endTime = -1;
    int64_t totalNum;
    database->QueryOperatorsTotalNum(requestParams, totalNum);
    int expectSize = 28;
    EXPECT_EQ(totalNum, expectSize);
}

TEST_F(TestSuit, QueryOperatorsTotalNumWithSize)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryOperatorParams requestParams;
    requestParams.rankId = "0";
    requestParams.orderName = "cann::Graph_";
    requestParams.minSize = 10; // min size = 10
    requestParams.maxSize = 1000; // max size = 1000
    requestParams.startTime = -1;
    requestParams.endTime = -1;
    int64_t totalNum;
    database->QueryOperatorsTotalNum(requestParams, totalNum);
    int expectSize = 15;
    EXPECT_EQ(totalNum, expectSize);
}

TEST_F(TestSuit, QueryOperatorsTotalNumWithTime)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryOperatorParams requestParams;
    requestParams.rankId = "0";
    requestParams.orderName = "cann::Graph_";
    requestParams.minSize = -1;
    requestParams.maxSize = -1;
    requestParams.startTime = 0;
    requestParams.endTime = 1695120000000; // end time = 1695120000000
    int64_t totalNum;
    database->QueryOperatorsTotalNum(requestParams, totalNum);
    int expectSize = 28;
    EXPECT_EQ(totalNum, expectSize);
}

TEST_F(TestSuit, QueryMemoryViewData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    Dic::Protocol::MemoryComponentParams requestParams;
    requestParams.rankId = "0";
    Dic::Protocol::OperatorMemory responseBody;
    database->QueryMemoryView(requestParams, responseBody);
    int expectSize = 5;
    EXPECT_EQ(responseBody.lines.size(), expectSize);
}

TEST_F(TestSuit, QueryOperatorSizeData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetMemoryDatabase("0");
    double min;
    double max;
    database->QueryOperatorSize(min, max);
    int expectMin = 64;
    int expectMax = 81984;
    EXPECT_EQ(min, expectMin);
    EXPECT_EQ(max, expectMax);
}
