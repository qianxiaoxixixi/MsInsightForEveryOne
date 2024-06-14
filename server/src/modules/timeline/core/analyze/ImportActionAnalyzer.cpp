/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "ImportActionAnalyzer.h"
namespace Dic::Module::Timeline {
ImportActionAnalyzer::ImportActionAnalyzer() = default;

void ImportActionAnalyzer::UpdateAllSimulationSliceDepthWithNoOverlap(
    std::vector<Protocol::SimpleSlice> &rowThreadTraceVec, const uint64_t trackId)
{
    SliceDepthCacheStruct sliceDepthCacheStruct;
    ComputeSimulationSliceDepth(rowThreadTraceVec, sliceDepthCacheStruct);
    sliceDepthCacheStruct.trackId = trackId;
    SliceDepthCacheManager::Instance().PutSliceDepthCacheStructByFileIdAndTrackId(trackId, sliceDepthCacheStruct);
}

void ImportActionAnalyzer::ComputeSimulationSliceDepth(std::vector<Protocol::SimpleSlice> &rowThreadTraceVec,
    SliceDepthCacheStruct &sliceDepthCacheStruct)
{
    std::vector<uint64_t> endList;
    std::unordered_map<uint64_t, int32_t> sliceIdAndDepthMap;
    for (auto &item : rowThreadTraceVec) {
        while (item.depth < endList.size() && endList[item.depth] > item.timestamp) {
            item.depth++;
        }
        if (item.depth < endList.size()) {
            endList[item.depth] = item.endTime;
        } else {
            endList.emplace_back(item.endTime);
        }
        sliceIdAndDepthMap[item.id] = item.depth;
    }
    sliceDepthCacheStruct.sliceIdAndDepthMap = sliceIdAndDepthMap;
    sliceDepthCacheStruct.maxDepTh = endList.size();
}
}