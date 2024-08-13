/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "HcclRepo_mock_data.h"
#include "CommucationOpTable.h"
#include "HcclRepo.h"
#include "TrackInfoManager.h"
using namespace Dic::TimeLine::HcclRepo::Mock;
class HcclRepoTest : public ::testing::Test {};
/**
 * 测试全量DB的hccl的group泳道的根据id空集合查询完整算子
 */
TEST_F(HcclRepoTest, test_QueryCompeteSliceByIds_group_track_with_empthIds)
{
    HcclRepo hcclRepo;
    SliceQuery sliceQuery;
    std::vector<uint64_t> sliceIds;
    std::vector<CompeteSliceDomain> competeSliceVec;
    hcclRepo.QueryCompeteSliceByIds(sliceQuery, sliceIds, competeSliceVec);
    EXPECT_EQ(competeSliceVec.size(), 0);
}

/**
 * 测试全量DB的hccl下调用栈算子id
 */
TEST_F(HcclRepoTest, test_QuerySliceIdsByCat_normal)
{
    HcclRepo hcclRepo;
    SliceQuery sliceQuery;
    std::vector<uint64_t> sliceIds;
    hcclRepo.QuerySliceIdsByCat(sliceQuery, sliceIds);
    EXPECT_EQ(sliceIds.size(), 0);
}

/**
 * 测试全量DB的hccl下调用栈算子个数
 */
TEST_F(HcclRepoTest, test_QueryPythonFunctionCountByTrackId_normal)
{
    HcclRepo hcclRepo;
    SliceQuery sliceQuery;
    uint64_t count = hcclRepo.QueryPythonFunctionCountByTrackId(sliceQuery);
    EXPECT_EQ(count, 0);
}

/**
 * 测试全量DB的hccl的group泳道的根据trackId查询所有简单算子，无对应track
 */
TEST_F(HcclRepoTest, test_QuerySimpleSliceWithOutNameByTrackId)
{
    HcclRepo hcclRepo;
    SliceQuery sliceQuery;
    std::vector<uint64_t> sliceIds;
    std::vector<SliceDomain> sliceVec;
    hcclRepo.QuerySimpleSliceWithOutNameByTrackId(sliceQuery, sliceVec);
    EXPECT_EQ(sliceVec.size(), 0);
}

/**
 * 测试全量DB的hccl的group泳道的根据id集合查询完整算子,无对应track
 */
TEST_F(HcclRepoTest, test_QueryCompeteSliceByIds_group_track_with_track_empty)
{
    HcclRepo hcclRepo;
    SliceQuery sliceQuery;
    std::vector<uint64_t> sliceIds = { 1, 2, 3 };
    std::vector<CompeteSliceDomain> competeSliceVec;
    hcclRepo.QueryCompeteSliceByIds(sliceQuery, sliceIds, competeSliceVec);
    EXPECT_EQ(competeSliceVec.size(), 0);
}

/**
 * 测试全量DB的hccl的group泳道的根据id集合查询完整算子,正常情况
 */
TEST_F(HcclRepoTest, test_QueryCompeteSliceByIds_group_track_with_normal)
{
    TrackInfoManager::Instance().Reset();
    class TableMock : public Dic::Module::Timeline::CommucationOpTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<CommucationTaskOpPO> &result) override
        {
            QueryCompeteSliceByIdsGroupTrackWithNormalExcuteQuery(fileId, result);
            ClearThreadLocal();
        }
    };
    std::unique_ptr<Dic::Module::Timeline::CommucationOpTable> ptr = std::make_unique<TableMock>();
    HcclRepo hcclRepo;
    hcclRepo.SetCommucationOpTable(std::move(ptr));
    SliceQuery sliceQuery;
    sliceQuery.trackId = TrackInfoManager::Instance().GetTrackId("999", "yyy", "999group");
    std::vector<uint64_t> sliceIds = { 1, 2, 3 };
    std::vector<CompeteSliceDomain> competeSliceVec;
    hcclRepo.QueryCompeteSliceByIds(sliceQuery, sliceIds, competeSliceVec);
    const uint64_t expectSize = 2;
    EXPECT_EQ(competeSliceVec.size(), expectSize);
    auto it = competeSliceVec.begin();
    const uint64_t firstTimestamp = 22;
    EXPECT_EQ(it->timestamp, firstTimestamp);
    it++;
    const uint64_t lastTimestamp = 23;
    EXPECT_EQ(it->timestamp, lastTimestamp);
    TrackInfoManager::Instance().Reset();
}

/**
 * 测试全量DB的hccl的plane泳道的根据id集合查询完整算子,正常情况
 */
TEST_F(HcclRepoTest, test_QueryCompeteSliceByIds_plane_track_with_normal)
{
    TrackInfoManager::Instance().Reset();
    class TaskMock : public Dic::Module::Timeline::TaskTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<TaskPO> &result) override
        {
            QueryCompeteSliceByIdsPlaneTrackWithTaskTableMock(fileId, result);
            ClearThreadLocal();
        }
    };
    class CommucationTaskInfoTableMock : public Dic::Module::Timeline::CommucationTaskInfoTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<CommucationTaskInfoPO> &result) override
        {
            QueryCompeteSliceByIdsPlaneTrackWithCommucationTaskInfoTableMock(fileId, result);
            ClearThreadLocal();
        }
    };
    std::unique_ptr<Dic::Module::Timeline::TaskTable> tPtr = std::make_unique<TaskMock>();
    std::unique_ptr<Dic::Module::Timeline::CommucationTaskInfoTable> cmPtr =
        std::make_unique<CommucationTaskInfoTableMock>();
    HcclRepo hcclRepo;
    hcclRepo.SetTaskTable(std::move(tPtr));
    hcclRepo.SetCommucationTaskInfoTable(std::move(cmPtr));
    SliceQuery sliceQuery;
    sliceQuery.trackId = TrackInfoManager::Instance().GetTrackId("999", "yyy", "999");
    std::vector<uint64_t> sliceIds = { 1, 2, 3 };
    std::vector<CompeteSliceDomain> competeSliceVec;
    hcclRepo.QueryCompeteSliceByIds(sliceQuery, sliceIds, competeSliceVec);
    const uint64_t expectSize = 2;
    EXPECT_EQ(competeSliceVec.size(), expectSize);
    auto it = competeSliceVec.begin();
    const uint64_t firstTimestamp = 15;
    EXPECT_EQ(it->timestamp, firstTimestamp);
    it++;
    const uint64_t lastTimestamp = 16;
    EXPECT_EQ(it->timestamp, lastTimestamp);
    TrackInfoManager::Instance().Reset();
}

/**
 * 测试全量DB的hccl的group泳道的根据trackId查询所有简单算子,正常情况
 */
TEST_F(HcclRepoTest, test_QuerySimpleSliceWithOutNameByTrackId_group_track_with_normal)
{
    TrackInfoManager::Instance().Reset();
    class TaskMock : public Dic::Module::Timeline::TaskTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<TaskPO> &result) override
        {
            QueryGlobalTaskIdsByRankWithTaskTableMock(fileId, result);
            ClearThreadLocal();
        }
    };
    class CommucationTaskInfoTableMock : public Dic::Module::Timeline::CommucationTaskInfoTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<CommucationTaskInfoPO> &result) override
        {
            QueryOpIdsByGlabalTaskIdsForCommucationTaskInfoTable(fileId, result);
            ClearThreadLocal();
        }
    };
    class CommucationOpTableMock : public Dic::Module::Timeline::CommucationOpTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<CommucationTaskOpPO> &result) override
        {
            QuerySimpleSliceByIdsGroupTrackForCommucationOpTable(fileId, result);
            ClearThreadLocal();
        }
    };
    std::unique_ptr<Dic::Module::Timeline::TaskTable> tPtr = std::make_unique<TaskMock>();
    std::unique_ptr<Dic::Module::Timeline::CommucationTaskInfoTable> cmPtr =
        std::make_unique<CommucationTaskInfoTableMock>();
    std::unique_ptr<Dic::Module::Timeline::CommucationOpTable> copPtr = std::make_unique<CommucationOpTableMock>();
    HcclRepo hcclRepo;
    hcclRepo.SetTaskTable(std::move(tPtr));
    hcclRepo.SetCommucationTaskInfoTable(std::move(cmPtr));
    hcclRepo.SetCommucationOpTable(std::move(copPtr));
    SliceQuery sliceQuery;
    sliceQuery.trackId = TrackInfoManager::Instance().GetTrackId("999", "yyy", "999group");
    std::vector<SliceDomain> sliceVec;
    hcclRepo.QuerySimpleSliceWithOutNameByTrackId(sliceQuery, sliceVec);
    const uint64_t expectSize = 2;
    EXPECT_EQ(sliceVec.size(), expectSize);
    auto it = sliceVec.begin();
    const uint64_t firstTimestamp = 22;
    EXPECT_EQ(it->timestamp, firstTimestamp);
    it++;
    const uint64_t lastTimestamp = 23;
    EXPECT_EQ(it->timestamp, lastTimestamp);
    TrackInfoManager::Instance().Reset();
}

/**
 * 测试全量DB的hccl的plane泳道的根据trackId查询所有简单算子,正常情况
 */
TEST_F(HcclRepoTest, test_QuerySimpleSliceWithOutNameByTrackId_plane_track_with_normal)
{
    TrackInfoManager::Instance().Reset();
    class TaskMock : public Dic::Module::Timeline::TaskTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<TaskPO> &result) override
        {
            QueryAllPlaneSliceForTaskTableMock(fileId, result);
            ClearThreadLocal();
        }
    };
    class CommucationTaskInfoTableMock : public Dic::Module::Timeline::CommucationTaskInfoTable {
    public:
        void ExcuteQuery(const std::string &fileId, std::vector<CommucationTaskInfoPO> &result) override
        {
            QueryOpIdsByGlabalTaskIdsForCommucationTaskInfoTable(fileId, result);
            ClearThreadLocal();
        }
    };
    std::unique_ptr<Dic::Module::Timeline::TaskTable> tPtr = std::make_unique<TaskMock>();
    std::unique_ptr<Dic::Module::Timeline::CommucationTaskInfoTable> cmPtr =
        std::make_unique<CommucationTaskInfoTableMock>();
    HcclRepo hcclRepo;
    hcclRepo.SetTaskTable(std::move(tPtr));
    hcclRepo.SetCommucationTaskInfoTable(std::move(cmPtr));
    SliceQuery sliceQuery;
    sliceQuery.trackId = TrackInfoManager::Instance().GetTrackId("999", "yyy", "999");
    std::vector<SliceDomain> sliceVec;
    hcclRepo.QuerySimpleSliceWithOutNameByTrackId(sliceQuery, sliceVec);
    const uint64_t expectSize = 2;
    EXPECT_EQ(sliceVec.size(), expectSize);
    auto it = sliceVec.begin();
    const uint64_t firstTimestamp = 28;
    EXPECT_EQ(it->timestamp, firstTimestamp);
    it++;
    const uint64_t lastTimestamp = 29;
    EXPECT_EQ(it->timestamp, lastTimestamp);
    TrackInfoManager::Instance().Reset();
}
