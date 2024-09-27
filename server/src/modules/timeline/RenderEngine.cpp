//
//  * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
//
#include "pch.h"
#include "DomainObject.h"
#include "ProtocolEnumUtil.h"
#include "RenderEngine.h"
namespace Dic::Module::Timeline {
void RenderEngine::SetDataEngineInterface(std::shared_ptr<DataEngineInterface> dataEngineInterface)
{
    dataEngine = dataEngineInterface;
}

void RenderEngine::QueryThreadTraces(const Protocol::UnitThreadTracesParams &requestParams,
    Protocol::UnitThreadTracesBody &responseBody, uint64_t minTimestamp, int64_t traceId)
{
    SliceQuery sliceQuery;
    sliceQuery.startTime = requestParams.startTime;
    sliceQuery.endTime = requestParams.endTime;
    sliceQuery.minTimestamp = minTimestamp;
    sliceQuery.isFilterPythonFunction = requestParams.isFilterPythonFunction;
    sliceQuery.cat = "python_function";
    sliceQuery.trackId = traceId;
    sliceQuery.rankId = requestParams.cardId;
    sliceQuery.metaType = Protocol::STR_TO_ENUM<PROCESS_TYPE>(requestParams.metaType).value();
    uint64_t maxDepth = 0;
    bool havePythonFunction = false;
    std::set<uint64_t> ids;
    std::map<uint64_t, uint32_t> depthMap;
    sliceAnalyzerPtr->SetRepository(dataEngine);
    sliceAnalyzerPtr->ComputeScreenSliceIds(sliceQuery, ids, maxDepth, havePythonFunction, depthMap);
    std::vector<CompeteSliceDomain> competeSliceVec;
    std::vector<uint64_t> sliceIds(ids.begin(), ids.end());
    dataEngine->QueryCompeteSliceByIds(sliceQuery, sliceIds, competeSliceVec);
    for (auto &item : competeSliceVec) {
        item.depth = depthMap[item.id];
    }
    std::sort(competeSliceVec.begin(), competeSliceVec.end(), std::less<CompeteSliceDomain>());
    for (auto &item : competeSliceVec) {
        bool isHide = requestParams.isHideFlagEvents && (item.name == "SET_FLAG" || item.name == "WAIT_FLAG");
        if (isHide) {
            continue;
        }
        Protocol::ThreadTraces threadTraces{};
        threadTraces.id = std::to_string(item.id);
        threadTraces.name = item.name;
        if (!(item.endTime >= item.timestamp && item.timestamp >= minTimestamp && item.endTime >= minTimestamp)) {
            continue;
        }
        threadTraces.duration = item.endTime - item.timestamp;
        threadTraces.startTime = item.timestamp - minTimestamp;
        threadTraces.endTime = item.endTime - minTimestamp;
        threadTraces.depth = depthMap[item.id];
        threadTraces.threadId = requestParams.threadId;
        threadTraces.cname = item.cname;
        while (responseBody.data.size() <= item.depth) {
            responseBody.data.emplace_back();
        }
        responseBody.data[item.depth].emplace_back(threadTraces);
    }
    responseBody.maxDepth = maxDepth;
    responseBody.havePythonFunction = havePythonFunction;
}
}
