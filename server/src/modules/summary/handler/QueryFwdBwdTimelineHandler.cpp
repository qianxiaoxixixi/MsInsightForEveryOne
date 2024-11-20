/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <algorithm>
#include "WsSessionManager.h"
#include "DataBaseManager.h"
#include "QueryFwdBwdTimelineHandler.h"

namespace Dic::Module::Summary {
using namespace Dic::Server;
using namespace Dic::Module::Timeline;
bool QueryFwdBwdTimelineHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
{
    auto &request = dynamic_cast<PipelineFwdBwdTimelineRequest &>(*requestPtr.get());
    std::unique_ptr<PipelineFwdBwdTimelineResponse> responsePtr = std::make_unique<PipelineFwdBwdTimelineResponse>();
    PipelineFwdBwdTimelineResponse &response = *responsePtr.get();
    SetBaseResponse(request, response);

    // check request parameter
    std::string err;
    if (!request.params.CheckParams(err)) {
        SendResponse(std::move(responsePtr), false, "Failed to query fwd/bwd timeline data due to error parameters.");
        return false;
    }
    std::vector<std::string> rankIds = StringUtil::SplitStringWithParenthesesByComma(request.params.stageId);
    if (rankIds.empty()) {
        SendResponse(std::move(responsePtr), false, "Failed to query fwd/bwd timeline data due to empty rank ids.");
        return false;
    }
    for (auto const &rankId : rankIds) {
        PipelineFwdBwdTimelineByRank data = {rankId, {}, {}};
        if (!QueryFwdBwdTimelineByRank(rankId, data, response.body)) {
            ServerLog::Warn("Failed to query fwd/bwd timeline data for rand ", rankId);
            data = {rankId, {}, {}};
        }
        response.body.rankLists.push_back(rankId);
        response.body.rankDataList.push_back(data);
    }
    SendResponse(std::move(responsePtr), true);
    return true;
}

// responseBody only for max/min time
bool QueryFwdBwdTimelineHandler::QueryFwdBwdTimelineByRank(const std::string& rankId,
    PipelineFwdBwdTimelineByRank &data, PipelineFwdBwdTimelineResponseBody &responseBody)
{
    auto database = DataBaseManager::Instance().GetTraceDatabase(rankId);
    if (database == nullptr) {
        ServerLog::Error("Failed to query fwd/bwd timeline data by rank due to null connection for database.");
        return false;
    }
    data.rankId = rankId;
    // 组装前反向数据
    PipelineFwdBwdTimelineByComponent fwdBwdData = {"FWD/BWD", {}};
    if (!database->QueryFwdBwdDataByFlow(0, rankId, fwdBwdData.traceList)) {
        ServerLog::Warn("Failed to query fwd/bwd detail trace data for rank ", rankId);
        fwdBwdData = {"FWD/BWD", {}};
    }

    if (!fwdBwdData.traceList.empty()) {
        auto first = fwdBwdData.traceList.at(0);
        auto last = fwdBwdData.traceList.at(fwdBwdData.traceList.size() - 1);
        responseBody.maxTime = std::max(responseBody.maxTime, last.startTime + last.duration);
        responseBody.minTime = std::min(responseBody.minTime, first.startTime);
    }

    data.componentDataList.push_back(fwdBwdData);

    // 组装P2P算子数据，再另一个MR里，带合并后补充此部分
    PipelineFwdBwdTimelineByComponent p2pOpData = {"P2P", {}};
    data.componentDataList.push_back(p2pOpData);
    return true;
}
} // Dic::Module::Summary