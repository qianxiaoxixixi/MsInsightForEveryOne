/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "../../FullDbTestSuit.cpp"
#include "TimelineTestUtil.h"

class DbTimelineTestSuit : FullDbTestSuit {
};

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceNameCountWithFuzzyMatch)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    int expectCount = 1;
    Dic::Protocol::SearchCountParams params;
    params.searchContent = "hcom";
    params.rankId = "2"; // cardId = 2

    auto count = database->SearchSliceNameCount(params);
    EXPECT_EQ(count, expectCount);
}

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceNameCountWithCaseMatch)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    int expectCount = 0;
    Dic::Protocol::SearchCountParams params;
    params.searchContent = "Hcom";
    params.rankId = "2"; // cardId = 2
    params.isMatchCase = true;

    auto count = database->SearchSliceNameCount(params);
    EXPECT_EQ(count, expectCount);
}

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceNameCountWithExactMatch)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    int expectCount = 3;
    Dic::Protocol::SearchCountParams params;
    params.searchContent = "aclnnInplaceadd";
    params.rankId = "2"; // cardId = 2
    params.isMatchExact = true;

    auto count = database->SearchSliceNameCount(params);
    EXPECT_EQ(count, expectCount);
}

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceNameCountWithCaseAndExactMatch)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    int expectCount = 0;
    Dic::Protocol::SearchCountParams params;
    params.searchContent = "aclnnInplaceadd";
    params.rankId = "2"; // cardId = 2
    params.isMatchExact = true;
    params.isMatchCase = true;

    auto count = database->SearchSliceNameCount(params);
    EXPECT_EQ(count, expectCount);
}

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceName)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::SearchSliceParams params;
    params.searchContent = "hcom";
    int index = 0;
    Dic::Protocol::SearchSliceBody body;

    body.rankId = "2"; // cardId = 2
    std::string expectPid = "Ascend Hardware";
    std::string expectTid = "8";
    uint64_t expectStartTime = 181306181;
    int32_t expectDepth = 0;
    uint64_t expectDuration = 51121;

    database->SearchSliceName(params, index, minTimestamp, body);
    EXPECT_EQ(body.pid, expectPid);
    EXPECT_EQ(body.tid, expectTid);
    EXPECT_EQ(body.startTime, expectStartTime);
    EXPECT_EQ(body.depth, expectDepth);
    EXPECT_EQ(body.duration, expectDuration);
}

TEST_F(FullDbTestSuit, FullDb_of_ThreadTraces)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::UnitThreadTracesBody body;

    Dic::Protocol::UnitThreadTracesParams params;
    params.startTime = 0;
    params.cardId = "2";
    params.processId = "Ascend Hardware";
    params.metaType = "Ascend Hardware";
    params.threadId = "8";
    params.endTime = 400000000; // endTime = 400000000
    params.timePerPx = 1;

    database->QueryThreadTraces(params, body, minTimestamp, 0);
    EXPECT_EQ(body.data.size(), 1);
    EXPECT_EQ(body.data[0].size(), 1);
    EXPECT_EQ(body.data[0][0].startTime, 181306181); // startTime = 181306181
    EXPECT_EQ(body.data[0][0].endTime, 181357302); // endTime = 181357302
    EXPECT_EQ(body.data[0][0].id, "49210");
    EXPECT_EQ(body.data[0][0].name, "hcom_allReduce__305_880_1");

    body.data.clear();
    params.metaType = "HCCL";
    params.threadId = "1";

    database->QueryThreadTraces(params, body, minTimestamp, 0);
    EXPECT_EQ(body.data.size(), 1);
    EXPECT_EQ(body.data[0].size(), 1);
    EXPECT_EQ(body.data[0][0].startTime, 175902293); // startTime = 175902293
    EXPECT_EQ(body.data[0][0].endTime, 175903813); // endTime = 175903813
    EXPECT_EQ(body.data[0][0].id, "46919");
    EXPECT_EQ(body.data[0][0].name, "Memcpy");

    body.data.clear();
    params.metaType = "CANN_API";
    params.threadId = "20000";
    params.processId = "11814731181473";

    database->QueryThreadTraces(params, body, minTimestamp, 0);
    EXPECT_EQ(body.data.size(), 2); // size = 2
    EXPECT_EQ(body.data[0].size(), 3); // size = 3
    EXPECT_EQ(body.data[0][0].startTime, 39360870); // startTime = 39360870
    EXPECT_EQ(body.data[0][0].endTime, 39363430); // endTime = 39363430
    EXPECT_EQ(body.data[0][0].id, "1");
    EXPECT_EQ(body.data[0][0].name, "aclrtGetDeviceCount");

    params.metaType = "HBM";
    EXPECT_EQ(database->QueryThreadTraces(params, body, minTimestamp, 0), false);
}

TEST_F(FullDbTestSuit, FullDb_of_QueryRankIds)
{
    auto database = std::dynamic_pointer_cast<DbTraceDataBase, Timeline::VirtualTraceDatabase>(
        Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb"));

    DataBaseManager::Instance().SetFileType(FileType::MS_PROF);
    auto rankIds = database->QueryRankId();
    EXPECT_EQ(rankIds.size(), 8); // size = 8
}

TEST_F(FullDbTestSuit, FullDb_of_ThreadTracesSummary)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::UnitThreadTracesSummaryBody body;

    Dic::Protocol::UnitThreadTracesSummaryParams params;
    params.startTime = 0;
    params.cardId = "2";
    params.processId = "Ascend Hardware";
    params.metaType = "Ascend Hardware";
    params.endTime = 400000000; // endTime = 400000000

    database->QueryThreadTracesSummary(params, body, minTimestamp);
    EXPECT_EQ(body.data.size(), 24); // size = 24
    EXPECT_EQ(body.data[0].startTime, 39828694); // startTime = 39828694

    body.data.clear();
    params.metaType = "HCCL";

    database->QueryThreadTracesSummary(params, body, minTimestamp);
    EXPECT_EQ(body.data.size(), 8); // size = 8
    EXPECT_EQ(body.data[0].startTime, 175902293); // startTime = 175902293

    body.data.clear();
    params.metaType = "CANN_API";
    params.processId = "11814731181473";

    database->QueryThreadTracesSummary(params, body, minTimestamp);
    EXPECT_EQ(body.data.size(), 3); // size = 3
    EXPECT_EQ(body.data[0].startTime, 39360870); // startTime = 39360870

    params.metaType = "HBM";
    EXPECT_EQ(database->QueryThreadTracesSummary(params, body, minTimestamp), false);
}

TEST_F(FullDbTestSuit, FullDb_of_ThreadTracesList)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::UnitThreadsBody body;

    Dic::Protocol::UnitThreadsParams params;
    params.startTime = 0;
    params.endTime = 400000000; // endTime = 400000000
    params.rankId = "2";
    Dic::Protocol::Metadata metadata{
            .tid = "8",
            .pid = "Ascend Hardware",
            .metaType = "Ascend Hardware"
    };
    params.metadataList.emplace_back(metadata);
    database->QueryThreads(params, body, minTimestamp, {0});
    EXPECT_EQ(body.data.size(), 1);
    EXPECT_EQ(body.data[0].title, "hcom_allReduce__305_880_1");
    EXPECT_EQ(body.data[0].selfTime, 51121); // selfTime = 51121
    EXPECT_EQ(body.data[0].occurrences, 1);

    body.data.clear();
    params.metadataList[0].tid = "1";
    params.metadataList[0].metaType = "HCCL";
    database->QueryThreads(params, body, minTimestamp, {0});
    EXPECT_EQ(body.data.size(), 1);
    EXPECT_EQ(body.data[0].title, "Memcpy");
    EXPECT_EQ(body.data[0].selfTime, 1520); // selfTime = 1520
    EXPECT_EQ(body.data[0].occurrences, 1);

    body.data.clear();
    params.metadataList[0].metaType = "CANN_API";
    params.metadataList[0].tid = "20000";
    params.metadataList[0].pid = "11814731181473";

    database->QueryThreads(params, body, minTimestamp, {0});
    EXPECT_EQ(body.data.size(), 3); // size = 3
    EXPECT_EQ(body.data[0].title, "aclrtGetDeviceCount");
    EXPECT_EQ(body.data[0].selfTime, 2560); // selfTime = 2560
    EXPECT_EQ(body.data[0].occurrences, 1);

    params.metadataList[0].metaType = "HBM";
    EXPECT_EQ(database->QueryThreads(params, body, minTimestamp, {0}), true);
}

TEST_F(FullDbTestSuit, FullDb_of_ThreadTraceDetail)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::UnitThreadDetailBody body;

    Dic::Protocol::ThreadDetailParams params;
    params.startTime = 39828694; // startTime = 39828694
    params.rankId = "2";
    params.pid = "Ascend Hardware";
    params.metaType = "Ascend Hardware";
    params.tid = "8";
    params.id = "0";

    database->QueryThreadDetail(params, body, minTimestamp, 0);
    EXPECT_EQ(body.data.title, "aclnnInplaceZero_ZerosLikeAiCore_ZerosLike");
    EXPECT_EQ(body.data.duration, 612092); // duration = 612092

    params.metaType = "HCCL";
    params.tid = "1";
    params.id = "46919";
    params.startTime = 175902293; // startTime = 175902293

    database->QueryThreadDetail(params, body, minTimestamp, 0);
    EXPECT_EQ(body.data.title, "Memcpy");
    EXPECT_EQ(body.data.duration, 1520); // duration = 1520

    params.metaType = "CANN_API";
    params.tid = "20000";
    params.pid = "11814731181473";
    params.id = "4";
    params.startTime = 39374331; // startTime = 39374331

    database->QueryThreadDetail(params, body, minTimestamp, 0);
    EXPECT_EQ(body.data.title, "aclrtMemcpy");
    EXPECT_EQ(body.data.duration, 379); // duration = 379

    params.metaType = "HBM";
    EXPECT_EQ(database->QueryThreadDetail(params, body, minTimestamp, 0), false);
}

TEST_F(FullDbTestSuit, FullDb_of_UnitMetaData)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");

    auto metaData = std::vector<std::unique_ptr<Protocol::UnitTrack>>();
    database->QueryUnitsMetadata("2", metaData);

    EXPECT_EQ(metaData.size(), 5); // size = 5
    EXPECT_EQ(metaData[0]->children.size(), 3); // size = 3
}

TEST_F(FullDbTestSuit, FullDb_of_HostMetaData)
{
    auto database = std::dynamic_pointer_cast<DbTraceDataBase, Timeline::VirtualTraceDatabase>(
        Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb"));

    auto metaData = std::vector<std::unique_ptr<Protocol::UnitTrack>>();
    database->QueryHostMetadata(metaData);

    EXPECT_EQ(metaData.size(), 1);
    EXPECT_EQ(metaData[0]->children.size(), 1);
}

TEST_F(FullDbTestSuit, FullDb_of_QueryKernelDetailData)
{
    Dic::Protocol::KernelDetailsParams requestParams;
    requestParams.current = 1;
    requestParams.pageSize = 20; // pageSize = 20
    requestParams.order = "ASC";
    requestParams.orderBy = "name";
    requestParams.coreType = "AI_VECTOR_CORE";
    requestParams.searchName = "aclnn";
    requestParams.rankId = "2";
    Dic::Protocol::KernelDetailsBody responseBody;
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = std::dynamic_pointer_cast<DbTraceDataBase, Timeline::VirtualTraceDatabase>(
        Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb"));

    database->QueryKernelDetailData(requestParams, responseBody, minTimestamp);

    EXPECT_EQ(responseBody.kernelDetails.size(), 11); // size = 11
    EXPECT_EQ(responseBody.acceleratorCoreList.size(), 1);
}

TEST_F(FullDbTestSuit, FullDb_of_UnitCounter)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");

    Dic::Protocol::UnitCounterParams params;
    params.startTime = 0;
    params.rankId = "2";
    params.pid = "HBM";
    params.metaType = "HBM";
    params.threadName = "HBM 1/Read";
    params.threadId = "1/Read";
    params.endTime = 400000000; // endTime = 400000000

    auto counterData = std::vector<Protocol::UnitCounterData>();
    database->QueryUnitCounter(params, minTimestamp, counterData);
    EXPECT_EQ(counterData.size(), 1);

    params.pid = "LLC";
    params.metaType = "LLC";
    params.threadName = "LLC 0 Read/Hit Rate";
    params.threadId = "0 Read/Hit Rate";
    counterData.clear();
    database->QueryUnitCounter(params, minTimestamp, counterData);
    EXPECT_EQ(counterData.size(), 3); // size = 3

    params.pid = "DDR";
    params.metaType = "DDR";
    params.threadName = "Read";
    params.threadId = "Read";
    counterData.clear();
    database->QueryUnitCounter(params, minTimestamp, counterData);
    EXPECT_EQ(counterData.size(), 5); // size = 5

    params.metaType = "Ascend Hardware";
    EXPECT_EQ(database->QueryUnitCounter(params, minTimestamp, counterData), false);
}

TEST_F(FullDbTestSuit, QueryEventsViewData4HostProcess)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::EventsViewParams params;
    params.currentPage = CUR_PAGE_1;
    params.metaType = "CANN_API";
    params.order = "descend";
    params.orderBy = "duration";
    params.pageSize = PAGE_SIZE;
    params.pid = "2750";
    params.processName = "process 2750";
    params.rankId = "2";

    Dic::Protocol::EventsViewBody body;
    database->QueryEventsViewData(params, body, minTimestamp);
    const uint64_t EXPECT_COUNT = 4;
    const uint64_t EXPECT_START = 39360870;
    const uint64_t EXPECT_DURATION = 2560;
    const uint64_t EXPECT_DEPTH = 0;

    EXPECT_EQ(body.count, EXPECT_COUNT);
    CheckEventsViewColumns4Api(body);
    EXPECT_EQ(body.eventDetailList.size(), EXPECT_COUNT);

    auto ptr = dynamic_cast<Dic::Protocol::HostEventDetail*>(body.eventDetailList.at(0).get());
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(ptr->name, "aclrtGetDeviceCount");
    EXPECT_EQ(ptr->startTime, EXPECT_START);
    EXPECT_EQ(ptr->duration, EXPECT_DURATION);
    EXPECT_EQ(ptr->tid, "3571117473");
    EXPECT_EQ(ptr->pid, "2750");
    EXPECT_EQ(ptr->processId, "11814731181473");
    EXPECT_EQ(ptr->threadId, "20000");
    EXPECT_EQ(ptr->depth, EXPECT_DEPTH);
}

TEST_F(FullDbTestSuit, QueryEventsViewData4HostProcessThread)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::EventsViewParams params;
    params.currentPage = CUR_PAGE_1;
    params.metaType = "CANN_API";
    params.order = "descend";
    params.orderBy = "duration";
    params.pageSize = PAGE_SIZE;
    params.pid = "11814731181473";
    params.processName = "Thread 3571117473";
    params.rankId = "2";

    Dic::Protocol::EventsViewBody body;
    database->QueryEventsViewData(params, body, minTimestamp);
    const uint64_t EXPECT_COUNT = 4;
    const uint64_t EXPECT_START = 39360870;
    const uint64_t EXPECT_DURATION = 2560;
    const uint64_t EXPECT_DEPTH = 0;

    EXPECT_EQ(body.count, EXPECT_COUNT);
    CheckEventsViewColumns4Api(body);
    EXPECT_EQ(body.eventDetailList.size(), EXPECT_COUNT);

    auto ptr = dynamic_cast<Dic::Protocol::HostEventDetail*>(body.eventDetailList.at(0).get());
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(ptr->name, "aclrtGetDeviceCount");
    EXPECT_EQ(ptr->startTime, EXPECT_START);
    EXPECT_EQ(ptr->duration, EXPECT_DURATION);
    EXPECT_EQ(ptr->tid, "3571117473");
    EXPECT_EQ(ptr->pid, "2750");
    EXPECT_EQ(ptr->processId, "11814731181473");
    EXPECT_EQ(ptr->threadId, "20000");
    EXPECT_EQ(ptr->depth, EXPECT_DEPTH);
}

TEST_F(FullDbTestSuit, QueryEventsViewData4HostProcessThreadCANN)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::EventsViewParams params;
    params.currentPage = CUR_PAGE_1;
    params.metaType = "CANN_API";
    params.order = "descend";
    params.orderBy = "duration";
    params.pageSize = PAGE_SIZE;
    params.pid = "11814731181473";
    params.processName = "CANN";
    params.rankId = "2";

    Dic::Protocol::EventsViewBody body;
    database->QueryEventsViewData(params, body, minTimestamp);
    const uint64_t EXPECT_COUNT = 4;
    const uint64_t EXPECT_START = 39360870;
    const uint64_t EXPECT_DURATION = 2560;
    const uint64_t EXPECT_DEPTH = 0;

    EXPECT_EQ(body.count, EXPECT_COUNT);
    CheckEventsViewColumns4Api(body);
    EXPECT_EQ(body.eventDetailList.size(), EXPECT_COUNT);

    auto ptr = dynamic_cast<Dic::Protocol::HostEventDetail*>(body.eventDetailList.at(0).get());
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(ptr->name, "aclrtGetDeviceCount");
    EXPECT_EQ(ptr->startTime, EXPECT_START);
    EXPECT_EQ(ptr->duration, EXPECT_DURATION);
    EXPECT_EQ(ptr->tid, "3571117473");
    EXPECT_EQ(ptr->pid, "2750");
    EXPECT_EQ(ptr->processId, "11814731181473");
    EXPECT_EQ(ptr->threadId, "20000");
    EXPECT_EQ(ptr->depth, EXPECT_DEPTH);
}

TEST_F(FullDbTestSuit, QueryEventsViewData4HostProcessThreadCANNAcl)
{
    const uint64_t minTimestamp = TraceTime::Instance().GetStartTime();
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    Dic::Protocol::EventsViewParams params;
    params.currentPage = CUR_PAGE_1;
    params.metaType = "CANN_API";
    params.order = "descend";
    params.orderBy = "duration";
    params.pageSize = PAGE_SIZE;
    params.pid = "11814731181473";
    params.processName = "CANN";
    params.rankId = "2";
    params.threadName = "acl";
    params.tid = "20000";

    Dic::Protocol::EventsViewBody body;
    database->QueryEventsViewData(params, body, minTimestamp);
    const uint64_t EXPECT_COUNT = 4;
    const uint64_t EXPECT_START = 39360870;
    const uint64_t EXPECT_DURATION = 2560;
    const uint64_t EXPECT_DEPTH = 0;

    EXPECT_EQ(body.count, EXPECT_COUNT);
    CheckEventsViewColumns4Api(body);
    EXPECT_EQ(body.eventDetailList.size(), EXPECT_COUNT);

    auto ptr = dynamic_cast<Dic::Protocol::HostEventDetail*>(body.eventDetailList.at(0).get());
    EXPECT_TRUE(ptr != nullptr);
    EXPECT_EQ(ptr->name, "aclrtGetDeviceCount");
    EXPECT_EQ(ptr->startTime, EXPECT_START);
    EXPECT_EQ(ptr->duration, EXPECT_DURATION);
    EXPECT_EQ(ptr->tid, "3571117473");
    EXPECT_EQ(ptr->pid, "2750");
    EXPECT_EQ(ptr->processId, "11814731181473");
    EXPECT_EQ(ptr->threadId, "20000");
    EXPECT_EQ(ptr->depth, EXPECT_DEPTH);
}