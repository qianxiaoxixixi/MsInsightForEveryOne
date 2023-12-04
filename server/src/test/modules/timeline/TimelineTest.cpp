/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "TimelineProtocolRequest.h"
#include "DataBaseManager.h"
#include "../../TestSuit.cpp"
#include "TraceTime.h"

class TimelineTest : TestSuit {
};

TEST_F(TestSuit, QueryPythonViewData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::SystemViewParams requestParams;
    Dic::Protocol::SystemViewBody responseBody;
    uint64_t PAGE = 10;
    requestParams.layer = "Python";
    requestParams.current = 1;
    requestParams.order = "descend";
    requestParams.orderBy = "name";
    requestParams.pageSize = PAGE;
    database->QueryPythonViewData(requestParams, responseBody);
    int expectSize = 10;
    EXPECT_EQ(responseBody.systemViewDetail.size(), expectSize);
}

TEST_F(TestSuit, QueryPythonViewWithTotalNum)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::SystemViewParams requestParams;
    Dic::Protocol::SystemViewBody responseBody;
    uint64_t PAGE = 10;
    requestParams.layer = "Python";
    requestParams.current = 1;
    requestParams.order = "descend";
    requestParams.orderBy = "name";
    requestParams.pageSize = PAGE;
    requestParams.isQueryTotal = true;
    database->QueryPythonViewData(requestParams, responseBody);
    int expectSize = 162;
    EXPECT_EQ(responseBody.total, expectSize);
}

TEST_F(TestSuit, QueryLayerOperatorTimeWithCann)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");

    const Dic::Module::Timeline::LayerStatData &data = database->QueryLayerData("CANN");
    double expectSize = 158266100;
    EXPECT_EQ(data.allOperatorTime, expectSize);
}

TEST_F(TestSuit, QueryLayerOperatorTimeWithPython)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");

    const Dic::Module::Timeline::LayerStatData &data = database->QueryLayerData("Python");
    double expectSize = 851869940;
    EXPECT_EQ(data.allOperatorTime, expectSize);
}

TEST_F(TestSuit, QueryLayerOperatorTimeWithAscend)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");

    const Dic::Module::Timeline::LayerStatData &data = database->QueryLayerData("Ascend Hardware");
    double expectSize = 842011262;
    EXPECT_EQ(data.allOperatorTime, expectSize);
    EXPECT_EQ(data.total, 57); // total operator = 57
}

TEST_F(TestSuit, QueryLayerOperatorTimeWithHCCL)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");

    const Dic::Module::Timeline::LayerStatData &data = database->QueryLayerData("HCCL");
    double expectSize = 449202040;
    EXPECT_EQ(data.allOperatorTime, expectSize);
}

TEST_F(TestSuit, QueryLayerOperatorTimeWithOverlap)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");

    const Dic::Module::Timeline::LayerStatData &data = database->QueryLayerData("Overlap Analysis");
    double expectSize = 445796394;
    EXPECT_EQ(data.allOperatorTime, expectSize);
}

TEST_F(TestSuit, QueryCoreType)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");

    const std::vector<std::string> &coreType = database->QueryCoreType();
    double expectSize = 7;
    EXPECT_EQ(coreType.size(), expectSize);
}

TEST_F(TestSuit, QueryKernelDetailData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::KernelDetailsParams requestParams;
    Dic::Protocol::KernelDetailsBody responseBody;
    uint64_t PAGE = 10;
    requestParams.current = 1;
    requestParams.order = "descend";
    requestParams.orderBy = "name";
    requestParams.pageSize = PAGE;
    requestParams.rankId = "0";
    database->QueryKernelDetailData(requestParams, responseBody, 0);
    int expectSize = 21;
    EXPECT_EQ(responseBody.count, expectSize);
}

TEST_F(TestSuit, QueryKernelDetailDataWithCoreType)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::KernelDetailsParams requestParams;
    Dic::Protocol::KernelDetailsBody responseBody;
    uint64_t PAGE = 10;
    requestParams.current = 1;
    requestParams.order = "descend";
    requestParams.orderBy = "name";
    requestParams.pageSize = PAGE;
    requestParams.rankId = "0";
    requestParams.coreType = "AI_CORE";
    database->QueryKernelDetailData(requestParams, responseBody, 0);
    int expectSize = 4;
    EXPECT_EQ(responseBody.count, expectSize);
}

TEST_F(TestSuit, QueryKernelDepthAndThread)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::KernelParams requestParams;
    Dic::Protocol::OneKernelBody responseBody;
    uint64_t DURATION = 72861;
    uint64_t TIMESTAMP = 1695115378736217088;
    requestParams.duration = DURATION;
    requestParams.name = "trans_Cast_15";
    requestParams.timestamp = 0;
    uint64_t minTime = TIMESTAMP;
    database->QueryKernelDepthAndThread(requestParams, responseBody, minTime);
    uint64_t depth = 0;
    uint64_t tid = 17;
    std::string pid = "300";
    EXPECT_EQ(responseBody.depth, depth);
    EXPECT_EQ(responseBody.threadId, tid);
    EXPECT_EQ(responseBody.pid, pid);
}

TEST_F(TestSuit, QueryCommunicationStatisticsData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::SummaryStatisticParams requestParams;
    Dic::Protocol::SummaryStatisticsBody responseBody;
    database->QueryCommunicationStatisticsData(requestParams, responseBody);
    EXPECT_EQ(responseBody.summaryStatisticsItemList.size(), 2); // item number = 2
}

TEST_F(TestSuit, QueryUnitCounterData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::UnitCounterParams requestParams;
    std::vector<Dic::Protocol::UnitCounterData> unitData;
    uint64_t STARTTIME = 1695115378653323520;
    uint64_t ENDTIME = 1695115378693307392;
    requestParams.pid = "14083661300";
    requestParams.threadName = "APP/DDR";
    requestParams.startTime = STARTTIME;
    requestParams.endTime = ENDTIME;
    uint64_t startTime = Dic::Module::Timeline::TraceTime::Instance().GetStartTime();
    database->QueryUnitCounter(requestParams, startTime, unitData);
    EXPECT_EQ(unitData.size(), 2); // unit data size = 2
    EXPECT_EQ(unitData[0].timestamp, 1695115378653323521); // timestamp = 1695115378653323521
}

TEST_F(TestSuit, QueryFlowCategoryEvents)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    Dic::Protocol::FlowCategoryEventsParams requestParams;
    std::vector<std::unique_ptr<Dic::Protocol::FlowEvent>> flowDetailList;
    uint64_t STARTTIME = 1695115378734066176;
    uint64_t ENDTIME = 1695115378743155968;
    requestParams.startTime = STARTTIME;
    requestParams.endTime = ENDTIME;
    requestParams.category = "HostToDevice";
    uint64_t startTime = Dic::Module::Timeline::TraceTime::Instance().GetStartTime();
    database->QueryFlowCategoryEvents(requestParams, startTime, flowDetailList);
    EXPECT_EQ(flowDetailList.size(), 25); // flowDetailList size = 25
    EXPECT_EQ(flowDetailList[0]->category, "HostToDevice");
    EXPECT_EQ(flowDetailList[0]->from.timestamp, 1695115378732879361); // timestamp = 1695115378732879361
    EXPECT_EQ(flowDetailList[0]->from.pid, "140836602");
    EXPECT_EQ(flowDetailList[0]->from.depth, 0);
    EXPECT_EQ(flowDetailList[0]->from.tid, 1413062); // from.tid = 1413062
    EXPECT_EQ(flowDetailList[0]->to.timestamp, 1695115378734473473); // to.timestamp = 1695115378734473473
    EXPECT_EQ(flowDetailList[0]->to.pid, "300");
    EXPECT_EQ(flowDetailList[0]->to.depth, 1);
    EXPECT_EQ(flowDetailList[0]->to.tid, 16); // to.tid = 16
}

TEST_F(TestSuit, QueryFlowCategoryList)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("0");
    std::vector<std::string> categories;
    database->QueryFlowCategoryList(categories);
    const int thirdNumber = 2;
    EXPECT_EQ(categories.size(), 3); // categories.size = 3
    EXPECT_EQ(categories[0], "HostToDevice");
    EXPECT_EQ(categories[1], "async_npu");
    EXPECT_EQ(categories[thirdNumber], "async_task_queue");
}
