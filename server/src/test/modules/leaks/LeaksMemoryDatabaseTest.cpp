/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include "MemoryDetailProtocolRequest.h"
#include "DataBaseManager.h"
#include "TraceTime.h"
#include "LeaksMemoryDatabase.h"
#include "LeaksMemoryService.h"
#include "LeaksMemoryTableColumn.h"
#include "../../TestSuit.cpp"

using namespace Dic::Module::Timeline;
using namespace Dic::Module::FullDb;
using namespace Dic;

class LeaksMemoryDatabaseTest : public ::testing::Test {
public:
    static void SetUpTestSuite()
    {
        std::string currPath = Dic::FileUtil::GetCurrPath();
        int index = currPath.find_last_of("server");
        currPath = currPath.substr(0, index + 1);
        std::string dbPath3 = R"(/src/test/test_data/full_db/)";
        DataBaseManager::Instance().SetDataType(DataType::DB);
        DataBaseManager::Instance().SetFileType(FileType::LEAKS);
        auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
        ASSERT_TRUE(memoryDatabase->OpenDb(currPath + dbPath3 + "leaks_dump_2025.dat", false));
        ASSERT_TRUE(memoryDatabase->DropMemoryAllocationAndBlockTable());
        ASSERT_TRUE(LeaksMemoryService::ParseMemoryLeaksDumpEvents("0"));
    }
    static void TearDownTestSuite()
    {
        auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
        memoryDatabase->CloseDb();
        DataBaseManager::Instance().Clear();
    }
};

TEST_F(LeaksMemoryDatabaseTest, QueryEntireEventsTable)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    std::vector<MemoryEvent> events;
    memoryDatabase->QueryEntireEventsTable(events);
    size_t expectSize = 6306;
    EXPECT_EQ(events.size(), expectSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryDeviceIds)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    std::set<std::string> deviceIds;
    memoryDatabase->QueryDeviceIds(deviceIds);
    size_t expectSize = 2;
    EXPECT_EQ(deviceIds.size(), expectSize);
}
TEST_F(LeaksMemoryDatabaseTest, QueryDeviceEventTypeMap)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    std::unordered_map<std::string, std::vector<std::string>> resultMap;
    memoryDatabase->QueryMallocOrFreeEventTypeWithDeviceId(resultMap);
    std::vector<std::string> eventTypes = resultMap["0"];
    auto it = std::find(eventTypes.begin(), eventTypes.end(), "HAL");
    EXPECT_TRUE(it != eventTypes.end());
    it = std::find(eventTypes.begin(), eventTypes.end(), "PTA");
    EXPECT_TRUE(it != eventTypes.end());
    eventTypes = resultMap["1"];
    it = std::find(eventTypes.begin(), eventTypes.end(), "HAL");
    EXPECT_TRUE(it != eventTypes.end());
    it = std::find(eventTypes.begin(), eventTypes.end(), "PTA");
    EXPECT_TRUE(it != eventTypes.end());
}
TEST_F(LeaksMemoryDatabaseTest, QueryMinAndMaxTimestamp)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    uint64_t minTimestamp = memoryDatabase->QueryMemoryEventExtremumTimestamp("0", true);
    uint64_t maxTimestamp = memoryDatabase->QueryMemoryEventExtremumTimestamp("0", false);
    const uint64_t expectMin = 1748933739689241190;
    const uint64_t expectMax = 1748933785530927010;
    EXPECT_EQ(minTimestamp, expectMin);
    EXPECT_EQ(maxTimestamp, expectMax);
}

TEST_F(LeaksMemoryDatabaseTest, QueryMemoryBlockWithNoTimeAndSizeCondition)
{
    std::vector<MemoryEvent> events;
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    LeaksMemoryBlockParams params;
    params.deviceId = "0";
    params.relativeTime = false;
    params.eventType = "PTA";
    std::vector<MemoryBlock> blocks;
    memoryDatabase->QueryMemoryBlocks(params, false, blocks);
    const size_t expectSize = 468;
    EXPECT_EQ(blocks.size(), expectSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryMemoryBlockWithTimeAndSizeConditionAndRelativeTime)
{
    LeaksMemoryBlockParams params;
    params.deviceId = "0";
    params.relativeTime = true;
    // 15s
    const uint64_t endTimestamp = 15000000000;
    const uint64_t maxSize = 60000;
    params.endTimestamp = endTimestamp;
    params.maxSize = maxSize;
    params.eventType = "PTA";
    params.orderBy = std::string(MemoryBlockTableColumn::START_TIMESTAMP);
    std::vector<MemoryBlock> blocks;
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    memoryDatabase->QueryMemoryBlocks(params, false, blocks);
    uint64_t minTimestamp = memoryDatabase->QueryMemoryEventExtremumTimestamp("0", true);
    const size_t expectSize = 228;
    EXPECT_EQ(blocks.size(), expectSize);
    MemoryBlock &firstBlock = blocks[0];
    const uint64_t expectStartTime = 7707721000;
    const uint64_t expectEndTime = 42462343790;
    EXPECT_EQ(firstBlock.startTimestamp, expectStartTime);
    EXPECT_EQ(firstBlock.endTimestamp, expectEndTime);
    EXPECT_TRUE(firstBlock.size > 0 and firstBlock.size < params.maxSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryAllocationWithNoTimeCondition)
{
    LeaksMemoryAllocationParams params;
    params.deviceId = "0";
    params.optimized = false;
    params.eventType = "PTA";
    std::vector<MemoryAllocation> allocations;
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    memoryDatabase->QueryMemoryAllocations(params, allocations);
    size_t expectSize = 936;
    EXPECT_EQ(allocations.size(), expectSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryAllocationWithTimeAndRelativeCondition)
{
    LeaksMemoryAllocationParams params;
    params.deviceId = "0";
    params.optimized = false;
    // 15s
    const uint64_t endTimestamp = 15000000000;
    params.endTimestamp = endTimestamp;
    params.relativeTime = true;
    params.eventType = "PTA";
    std::vector<MemoryAllocation> allocations;
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    memoryDatabase->QueryMemoryAllocations(params, allocations);
    const size_t expectSize = 243;
    EXPECT_EQ(allocations.size(), expectSize);
    MemoryAllocation &firstAllocation = allocations[0];
    const uint64_t expectTimestamp = 7707721000;
    EXPECT_EQ(firstAllocation.timestamp, expectTimestamp);
    uint64_t totalSize = 7168;
    EXPECT_EQ(firstAllocation.totalSize, totalSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryLatestAllocationWithinTimestamp)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    const std::string deviceId = "0";
    const std::string eventType = "HAL";
    const uint64_t expectDuration = 10000000;
    const uint64_t timestamp =
            memoryDatabase->QueryMemoryEventExtremumTimestamp(deviceId, true) + expectDuration;
    auto alloc = memoryDatabase->QueryLatestAllocationWithinTimestamp(deviceId, eventType, timestamp);
    EXPECT_TRUE(alloc.has_value());
    const uint64_t expectTotalSize = 8470016;
    EXPECT_EQ(alloc->totalSize, expectTotalSize);
}


TEST_F(LeaksMemoryDatabaseTest, QueryNextAllocationAfterTimestamp)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    const std::string deviceId = "0";
    const std::string eventType = "PTA";
    const uint64_t expectDuration = 10000000;
    const uint64_t timestamp =
            memoryDatabase->QueryMemoryEventExtremumTimestamp(deviceId, true) + expectDuration;
    auto alloc = memoryDatabase->QueryNextAllocationAfterTimestamp(deviceId, eventType, timestamp);
    EXPECT_TRUE(alloc.has_value());
    const uint64_t expectTotalSize = 7168;
    EXPECT_EQ(alloc->totalSize, expectTotalSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryMemoryBlocksOwnersReleasedAfterTimestamp)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    const std::string deviceId = "0";
    const std::string eventType = "PTA";
    // 10s
    const uint64_t expectDuration = 10000000000;
    const uint64_t timestamp =
            memoryDatabase->QueryMemoryEventExtremumTimestamp(deviceId, true) + expectDuration;
    std::set<std::string> owners;
    memoryDatabase->QueryMemoryBlocksOwnersReleasedAfterTimestamp(deviceId, eventType, timestamp, owners);
    EXPECT_FALSE(owners.empty());
    std::set<std::string> expectOwners = {
            "PTA",
            "PTA@model@weight",
    };
    EXPECT_EQ(owners, expectOwners);
}

TEST_F(LeaksMemoryDatabaseTest, QueryTotalSizeUtilTimestampUsingOwner)
{
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    const std::string deviceId = "0";
    // 10s
    const uint64_t expectDuration = 10000000000;
    const std::string eventType = "HAL";
    const uint64_t timestamp =
            memoryDatabase->QueryMemoryEventExtremumTimestamp(deviceId, true) + expectDuration;
    auto allocation =
            memoryDatabase->QueryLatestAllocationWithinTimestamp(deviceId, eventType, timestamp);
    uint64_t cannTotalSize = memoryDatabase->QueryTotalSizeUntilTimestampUsingOwner(deviceId, timestamp, "CANN");
    uint64_t ptaTotalSize = memoryDatabase->
            QueryTotalSizeUntilTimestampUsingOwner(deviceId, timestamp, "PTA");
    uint64_t ptaModelTotalSize = memoryDatabase->
            QueryTotalSizeUntilTimestampUsingOwner(deviceId, timestamp, "PTA@model");
    uint64_t ptaOpTotalSize = memoryDatabase->
            QueryTotalSizeUntilTimestampUsingOwner(deviceId, timestamp, "PTA@ops");
    const uint64_t expectCANNTotalSize = 31526918;
    const uint64_t expectPTATotalSize = 3542016;
    const uint64_t expectPTAModelTotalSize = 3072;
    const uint64_t expectPTAOpTotalSize = 0;
    EXPECT_TRUE(allocation.has_value());
    EXPECT_EQ(allocation->totalSize, cannTotalSize);
    EXPECT_EQ(cannTotalSize, expectCANNTotalSize);
    EXPECT_EQ(ptaTotalSize, expectPTATotalSize);
    EXPECT_EQ(ptaModelTotalSize, expectPTAModelTotalSize);
    EXPECT_EQ(ptaOpTotalSize, expectPTAOpTotalSize);
}

TEST_F(LeaksMemoryDatabaseTest, QueryPythonTraces)
{
    const uint64_t startTimestamp = 1000000;
    const uint64_t endTimestamp = 1000000000000;
    LeaksMemoryThreadPythonTraceParams params;
    params.startTimestamp = startTimestamp;
    params.endTimestamp = endTimestamp;
    params.relativeTime = true;
    params.deviceId = '0';
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    ASSERT_TRUE(memoryDatabase != nullptr);
    std::vector<uint64_t> threadIds;
    const int expectThreadIdSize = 4;
    memoryDatabase->QueryThreadIds(threadIds);
    EXPECT_EQ(threadIds.size(), expectThreadIdSize);
    params.threadId = threadIds[0];
    LeaksMemoryPythonTrace trace;
    memoryDatabase->QueryPythonTrace(params, trace);
    EXPECT_FALSE(trace.slices.empty());
}
/***
 * 测试通过简易请求(仅deviceId)查询内存事件  应返回该deviceId下的所有事件
 */
TEST_F(LeaksMemoryDatabaseTest, QueryMemoryEventsTableWithSimpleParams)
{
    LeaksMemoryEventParams simpleQueryParams;
    simpleQueryParams.deviceId = "0";
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    ASSERT_TRUE(memoryDatabase != nullptr);
    std::vector<MemoryEvent> events;
    int64_t totalSize = memoryDatabase->QueryEventsByRequestParams(simpleQueryParams, events);
    int64_t expectTotalSize = 3111;
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(events.size(), totalSize);
    events.clear();
    simpleQueryParams.deviceId = "1";
    expectTotalSize = 3195;
    totalSize = memoryDatabase->QueryEventsByRequestParams(simpleQueryParams, events);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(events.size(), totalSize);
}

/***
* 测试通过时间范围、字段、分页、排序参数请求内存事件
*/
TEST_F(LeaksMemoryDatabaseTest, QueryMemoryEventsTableWithComplexParams)
{
    LeaksMemoryEventParams complexParams;
    // 测试过滤后总量大于pageSize的情况，且根据Timestamp升序排序
    complexParams.deviceId = "0";
    complexParams.endTimestamp = 10000000000;
    complexParams.relativeTime = true;
    complexParams.currentPage = 1;
    complexParams.pageSize = 20;
    complexParams.orderBy = std::string(MemoryEventTableColumn::TIMESTAMP);
    int64_t expectTotalSize = 1262;
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    ASSERT_TRUE(memoryDatabase != nullptr);
    std::vector<MemoryEvent> events;
    int64_t totalSize = memoryDatabase->QueryEventsByRequestParams(complexParams, events);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(events.size(), complexParams.pageSize);
    EXPECT_EQ(events[0].timestamp, 0);
    // 测试过滤后总量小于pageSize的情况，且根据Ptr降序排序
    complexParams.endTimestamp = 1000000000;
    complexParams.orderBy = std::string(MemoryEventTableColumn::PTR);
    complexParams.desc = true;
    expectTotalSize = 12;
    uint64_t expectFirstEventTimestamp = 21134460;
    events.clear();
    totalSize = memoryDatabase->QueryEventsByRequestParams(complexParams, events);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(events.size(), expectTotalSize);
    EXPECT_EQ(events[0].timestamp, expectFirstEventTimestamp);
    // 测试过滤后总量大于pageSize的情况，包括分页参数、filter、orderBy
    complexParams.endTimestamp = 10000000000;
    complexParams.filters.emplace(std::string(MemoryEventTableColumn::ATTR), "PTA");
    expectTotalSize = 211;
    events.clear();
    totalSize = memoryDatabase->QueryEventsByRequestParams(complexParams, events);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(events.size(), complexParams.pageSize);
    expectFirstEventTimestamp = 8626530200;
    EXPECT_EQ(events[0].timestamp, expectFirstEventTimestamp);
}

// Memory Block 数据表相关查询

/***
* 测试查询某个deviceId+eventType下的全部数据
*/
TEST_F(LeaksMemoryDatabaseTest, QueryMemoryBlockTablesWithTimeRangeParams)
{
    LeaksMemoryBlockParams simpleQueryParams;
    simpleQueryParams.deviceId = "0";
    simpleQueryParams.eventType = "PTA";
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    ASSERT_TRUE(memoryDatabase != nullptr);
    std::vector<MemoryBlock> blocks;
    int64_t totalSize = memoryDatabase->QueryMemoryBlocks(simpleQueryParams, true, blocks);
    int64_t expectTotalSize = 468;
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(blocks.size(), totalSize);
    blocks.clear();
    simpleQueryParams.deviceId = "1";
    simpleQueryParams.eventType = "HAL";
    expectTotalSize = 80;
    totalSize = memoryDatabase->QueryMemoryBlocks(simpleQueryParams, true, blocks);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(blocks.size(), totalSize);
}

/***
* 测试通过时间范围、字段、分页、排序参数请求内存事件
*/
TEST_F(LeaksMemoryDatabaseTest, QueryMemoryEventsTableWithPaginationParams)
{
    LeaksMemoryBlockParams complexParams;
    // 测试过滤后总量大于pageSize的情况，且根据startTimestamp升序排序
    complexParams.deviceId = "0";
    complexParams.eventType = "PTA";
    complexParams.endTimestamp = 10000000000;
    complexParams.relativeTime = true;
    complexParams.currentPage = 1;
    complexParams.pageSize = 20;
    complexParams.orderBy = std::string(MemoryBlockTableColumn::START_TIMESTAMP);
    int64_t expectTotalSize = 211;
    uint64_t expectFirstBlockStartTimestamp = 7707721000;
    auto memoryDatabase = DataBaseManager::Instance().GetLeaksMemoryDatabase("0");
    ASSERT_TRUE(memoryDatabase != nullptr);
    std::vector<MemoryBlock> blocks;
    int64_t totalSize = memoryDatabase->QueryMemoryBlocks(complexParams, true, blocks);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(blocks.size(), complexParams.pageSize);
    EXPECT_EQ(blocks[0].startTimestamp, expectFirstBlockStartTimestamp);
    // 测试过滤后总量小于pageSize的情况，且根据Ptr降序排序
    complexParams.endTimestamp = 7838050340;
    complexParams.orderBy = std::string(MemoryBlockTableColumn::ADDR);
    complexParams.desc = true;
    expectTotalSize = 9;
    expectFirstBlockStartTimestamp = 7838050340;
    blocks.clear();
    totalSize = memoryDatabase->QueryMemoryBlocks(complexParams, true, blocks);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(blocks.size(), expectTotalSize);
    EXPECT_EQ(blocks[0].startTimestamp, expectFirstBlockStartTimestamp);
    // 测试过滤后总量大于pageSize的情况，包括分页参数、filter、orderBy
    complexParams.endTimestamp = 10000000000;
    complexParams.filters.emplace(std::string(MemoryBlockTableColumn::ADDR), "24");
    expectTotalSize = 21;
    expectFirstBlockStartTimestamp = 8625992740;
    blocks.clear();
    totalSize = memoryDatabase->QueryMemoryBlocks(complexParams, true, blocks);
    EXPECT_EQ(totalSize, expectTotalSize);
    EXPECT_EQ(blocks.size(), complexParams.pageSize);
    EXPECT_EQ(blocks[0].startTimestamp, expectFirstBlockStartTimestamp);
}