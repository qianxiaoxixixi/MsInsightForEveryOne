/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "SliceTable.h"
#include "FlowTable.h"
#include "ThreadTable.h"
#include "Repository.h"
namespace Dic::Module::Timeline {
void Repository::QuerySimpleSliceWithOutNameByTrackId(const SliceQuery &sliceQuery, std::vector<SliceDomain> &sliceVec)
{
    SliceTable sliceTable;
    std::vector<SlicePO> tempSlicePOVec;
    sliceTable.Select(SliceColumn::ID, SliceColumn::TIMESTAMP, SliceColumn::ENDTIME)
        .Eq(SliceColumn::TRACKID, sliceQuery.trackId)
        .OrderBy(SliceColumn::TIMESTAMP, TableOrder::ASC)
        .OrderBy(SliceColumn::ID, TableOrder::ASC)
        .ExcuteQuery(sliceQuery.db, tempSlicePOVec);
    for (const auto &item : tempSlicePOVec) {
        SliceDomain cachelice;
        cachelice.id = item.id;
        cachelice.timestamp = item.timestamp;
        cachelice.endTime = item.endTime;
        sliceVec.emplace_back(cachelice);
    }
}

void Repository::QuerySliceIdsByCat(const SliceQuery &sliceQuery, std::vector<uint64_t> &sliceIds)
{
    SliceTable sliceTable;
    std::vector<SlicePO> slicePOVec;
    sliceTable.Select(SliceColumn::ID)
        .Eq(SliceColumn::TRACKID, sliceQuery.trackId)
        .Eq(SliceColumn::CAT, sliceQuery.cat)
        .OrderBy(SliceColumn::ID, TableOrder::ASC)
        .ExcuteQuery(sliceQuery.db, slicePOVec);
    for (const auto &item : slicePOVec) {
        sliceIds.emplace_back(item.id);
    }
}

uint64_t Repository::QueryPythonFunctionCountByTrackId(const SliceQuery &sliceQuery)
{
    SliceTable sliceTable;
    uint64_t count = sliceTable.Eq(SliceColumn::TRACKID, sliceQuery.trackId)
                         .Eq(SliceColumn::CAT, sliceQuery.cat)
                         .Count(sliceQuery.db);
    return count;
}

void Repository::QueryCompeteSliceVecByTimeRangeAndTrackId(const SliceQuery &sliceQuery,
    std::vector<CompeteSliceDomain> &sliceVec)
{
    SliceTable sliceTable;
    std::vector<SlicePO> slicePOVec;
    sliceTable.Select(SliceColumn::ID, SliceColumn::TIMESTAMP)
        .Select(SliceColumn::DURATION, SliceColumn::ENDTIME, SliceColumn::NAME)
        .Eq(SliceColumn::TRACKID, sliceQuery.trackId)
        .LessEq(SliceColumn::TIMESTAMP, sliceQuery.endTime + sliceQuery.minTimestamp)
        .Greater(SliceColumn::ENDTIME, sliceQuery.startTime + sliceQuery.minTimestamp)
        .ExcuteQuery(sliceQuery.db, slicePOVec);
    for (const auto &item : slicePOVec) {
        CompeteSliceDomain temp;
        temp.id = item.id;
        temp.timestamp = item.timestamp;
        temp.duration = item.duration;
        temp.endTime = item.endTime;
        temp.name = item.name;
        sliceVec.emplace_back(std::move(temp));
    }
}

void Repository::QueryFlowPointByTimeRange(const FlowQuery &flowQuery, std::vector<FlowPoint> &flowPointVec)
{
    FlowTable flowTable;
    std::vector<FlowPO> flowPOVec;
    flowTable.Select(FlowColumn::TYPE, FlowColumn::TIMESTAMP, FlowColumn::FLOW_ID)
        .GreaterEq(FlowColumn::TIMESTAMP, flowQuery.startTime + flowQuery.minTimestamp)
        .LessEq(FlowColumn::TIMESTAMP, flowQuery.endTime + flowQuery.minTimestamp)
        .Eq(FlowColumn::TRACK_ID, flowQuery.trackId)
        .GroupBy(FlowColumn::FLOW_ID)
        .ExcuteQuery(flowQuery.db, flowPOVec);
    for (const auto &item : flowPOVec) {
        FlowPoint flowPoint;
        flowPoint.timestamp = item.timestamp;
        flowPoint.type = item.type;
        flowPoint.flowId = item.flowId;
        flowPointVec.emplace_back(std::move(flowPoint));
    }
}

void Repository::QueryFlowPointByFlowId(const FlowQuery &flowQuery, std::vector<FlowPoint> &flowPointVec)
{
    FlowTable flowTable;
    std::vector<FlowPO> flowPOVec;
    flowTable.Select(FlowColumn::NAME, FlowColumn::CAT, FlowColumn::FLOW_ID)
        .Select(FlowColumn::TIMESTAMP, FlowColumn::TYPE, FlowColumn::TRACK_ID)
        .Eq(FlowColumn::FLOW_ID, flowQuery.flowId)
        .ExcuteQuery(flowQuery.db, flowPOVec);
    for (const auto &item : flowPOVec) {
        FlowPoint flowPoint;
        flowPoint.name = item.name;
        flowPoint.cat = item.cat;
        flowPoint.flowId = item.flowId;
        flowPoint.timestamp = item.timestamp;
        flowPoint.type = item.type;
        flowPoint.trackId = item.trackId;
        flowPointVec.emplace_back(std::move(flowPoint));
    }
}

void Repository::QueryAllThreadInfo(const ThreadQuery &threadQuery,
    std::unordered_map<uint64_t, std::pair<std::string, std::string>> &threadInfo)
{
    ThreadTable threadTable;
    std::vector<ThreadPO> threadPOVec;
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::PID, ThreadColumn::TID)
        .ExcuteQuery(threadQuery.db, threadPOVec);
    for (const auto &item : threadPOVec) {
        threadInfo[item.trackId] = {item.pid, item.tid};
    }
}
}
