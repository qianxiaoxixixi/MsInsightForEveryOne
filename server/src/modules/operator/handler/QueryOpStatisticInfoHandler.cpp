/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#include <unordered_map>
#include <algorithm>
#include "pch.h"
#include "DataBaseManager.h"
#include "OperatorProtocolRequest.h"
#include "OperatorProtocolResponse.h"
#include "OperatorGroupConverter.h"
#include "WsSessionManager.h"
#include "OperatorProtocol.h"
#include "QueryOpStatisticInfoHandler.h"

namespace Dic::Module::Operator {
    using namespace Dic::Server;

    void QueryOpStatisticInfoHandler::HandleRequest(std::unique_ptr<Protocol::Request> requestPtr)
    {
        OperatorStatisticInfoRequest &request = dynamic_cast<OperatorStatisticInfoRequest &>(*requestPtr);
        WsSession &session = *WsSessionManager::Instance().GetSession();
        std::unique_ptr<OperatorStatisticInfoResponse> responsePtr = std::make_unique<OperatorStatisticInfoResponse>();
        OperatorStatisticInfoResponse &response = *responsePtr;

        bool rst = false;
        std::string errorMsg;
        if ((request.params.CommonCheck(errorMsg) && request.params.StatisticGroupCheck(errorMsg))) {
            rst = request.params.isCompare ?
                HandleStatisticcDataRequest(request, dynamic_cast<OperatorStatisticInfoResponse &>(*responsePtr)) :
                HandleStatisticcDataRequest(request, dynamic_cast<OperatorStatisticInfoResponse &>(*responsePtr));
        }

        SetBaseResponse(request, response);
        SetResponseResult(response, rst);
        session.OnResponse(std::move(responsePtr));
    }

    bool QueryOpStatisticInfoHandler::HandleCompareDataRequest(OperatorStatisticInfoRequest &request,
                                                               OperatorStatisticInfoResponse &response)
    {
        std::string rankId = Summary::VirtualSummaryDataBase::GetFileIdFromCombinationId(request.params.rankId);
        auto database = Timeline::DataBaseManager::Instance().GetSummaryDatabase(rankId);
        std::vector<Protocol::OperatorStatisticInfoRes> compareRes;
        if (!database->QueryAllOperatorStatisticInfo(response.total, request.params, compareRes)) {
            ServerLog::Error("[Operator]Failed to query Statistic Info, RankId = ", rankId);
            return false;
        }
        std::vector<Protocol::OperatorStatisticCmpInfoRes> res;
        res = CalCompareInfo(request.params, compareRes, compareRes);
        response.datas = res;
        return true;
    }

    bool QueryOpStatisticInfoHandler::HandleStatisticcDataRequest(OperatorStatisticInfoRequest &request,
                                                                  OperatorStatisticInfoResponse &response)
    {
        std::string rankId = Summary::VirtualSummaryDataBase::GetFileIdFromCombinationId(request.params.rankId);
        auto database = Timeline::DataBaseManager::Instance().GetSummaryDatabase(rankId);
        if (!database->QueryOperatorStatisticInfo(request.params, response)) {
            ServerLog::Error("[Operator]Failed to query Statistic Info, RankId = ", rankId);
            return false;
        }
        return true;
    }

    std::string QueryOpStatisticInfoHandler::GetGroup(const std::string &paramsGroup, OperatorStatisticInfoRes &data)
    {
        OperatorGroupConverter::OperatorGroup operatorGroup = Protocol::OperatorGroupConverter::ToEnum(paramsGroup);
        switch (operatorGroup) {
            case OperatorGroupConverter::OperatorGroup::OP_TYPE_GROUP:
            case OperatorGroupConverter::OperatorGroup::HCCL_TYPE_GROUP:
                return data.opType + data.accCore;
            case OperatorGroupConverter::OperatorGroup::OP_INPUT_SHAPE_GROUP:
                return data.opName + data.inputShape + data.accCore;
            default:
                return "";
        }
    }

    void QueryOpStatisticInfoHandler::GroupingData(const std::string &paramsGroup, OpStaticResVec &datFromDb,
        std::map<std::string, Protocol::OperatorStatisticCmpInfoRes> &groupMap, bool isBaselineData)
    {
        std::string group;
        for (auto &data : datFromDb) {
            group = GetGroup(paramsGroup, data);
            if (group.empty()) {
                continue;
            }
            if (isBaselineData) {
                groupMap[group].baseline = data;
            } else {
                groupMap[group].compare = data;
            }
        }
    }

    void QueryOpStatisticInfoHandler::SetOpInputShapeGroupData(OperatorStatisticCmpInfoRes &data)
    {
        if (data.compare.count == INT_MIN_VALUE) {
            data.diff.opName = data.baseline.opName;
            data.diff.inputShape = data.baseline.inputShape;
            data.diff.accCore = data.baseline.accCore;
        } else if (data.baseline.count == INT_MIN_VALUE) {
            data.diff.opName = data.compare.opName;
            data.diff.inputShape = data.compare.inputShape;
            data.diff.accCore = data.compare.accCore;
        } else {
            data.diff.opType = data.compare.opType + " -> " + data.compare.opType;
            data.diff.opName = data.compare.opName;
            data.diff.inputShape = data.compare.inputShape;
            data.diff.accCore = data.compare.accCore;
            data.diff.totalTime = data.compare.totalTime - data.baseline.totalTime;
            data.diff.count = data.compare.count - data.baseline.count;
            data.diff.avgTime = data.compare.avgTime - data.baseline.avgTime;
            data.diff.maxTime = data.compare.maxTime - data.baseline.maxTime;
            data.diff.minTime = data.compare.minTime - data.baseline.minTime;
        }
    }

    void QueryOpStatisticInfoHandler::SetOpOrHcclTypeGroupData(OperatorStatisticCmpInfoRes &data)
    {
        if (data.compare.count == INT_MIN_VALUE) {
            data.diff.opType = data.baseline.opType;
            data.diff.accCore = data.baseline.accCore;
        } else if (data.baseline.count == INT_MIN_VALUE) {
            data.diff.opType = data.compare.opType;
            data.diff.accCore = data.compare.accCore;
        } else {
            data.diff.opType = data.compare.opType;
            data.diff.opName = data.compare.opName + "->" + data.baseline.opName;
            data.diff.inputShape = data.compare.inputShape + "->" + data.baseline.inputShape;
            data.diff.accCore = data.compare.accCore;
            data.diff.totalTime = data.compare.totalTime - data.baseline.totalTime;
            data.diff.count = data.compare.count - data.baseline.count;
            data.diff.avgTime = data.compare.avgTime - data.baseline.avgTime;
            data.diff.maxTime = data.compare.maxTime - data.baseline.maxTime;
            data.diff.minTime = data.compare.minTime - data.baseline.minTime;
        }
    }

    void QueryOpStatisticInfoHandler::SetCommonDataByGroup(const std::string &paramsGroup,
                                                           OperatorStatisticCmpInfoRes &data)
    {
        OperatorGroupConverter::OperatorGroup operatorGroup = Protocol::OperatorGroupConverter::ToEnum(paramsGroup);
        switch (operatorGroup) {
            case OperatorGroupConverter::OperatorGroup::OP_TYPE_GROUP:
            case OperatorGroupConverter::OperatorGroup::HCCL_TYPE_GROUP:
                SetOpOrHcclTypeGroupData(data);
                break;
            case OperatorGroupConverter::OperatorGroup::OP_INPUT_SHAPE_GROUP:
                SetOpInputShapeGroupData(data);
                break;
            default:
                break;
        }
    }

    void QueryOpStatisticInfoHandler::CalDiffData(const std::string &paramsGroup,
        std::map<std::string, Protocol::OperatorStatisticCmpInfoRes> &groupMap,
        std::vector<Protocol::OperatorStatisticCmpInfoRes> &cmpRes)
    {
        std::vector<Protocol::OperatorStatisticCmpInfoRes> calDiffRes;
        for (auto &dataMap : groupMap) {
            // 其中一个不存在，不做比较
            if (dataMap.second.compare.count == INT_MIN_VALUE && dataMap.second.baseline.count == INT_MIN_VALUE) {
                continue;
            }
            SetCommonDataByGroup(paramsGroup, dataMap.second);
            cmpRes.emplace_back(dataMap.second);
        }
    }

    std::vector<Protocol::OperatorStatisticCmpInfoRes> QueryOpStatisticInfoHandler::CalCompareInfo(
        Protocol::OperatorStatisticReqParams &reqParams, OpStaticResVec &base, OpStaticResVec &cmp)
    {
        std::map<std::string, Protocol::OperatorStatisticCmpInfoRes> groupMap;
        std::vector<Protocol::OperatorStatisticCmpInfoRes> cmpRes;
        // 处理base cmp放在map里
        GroupingData(reqParams.group, base, groupMap, true);
        GroupingData(reqParams.group, cmp, groupMap, false);
        CalDiffData(reqParams.group, groupMap, cmpRes);

        // 对差值排序
        std::sort(cmpRes.begin(), cmpRes.end(), [](const auto& a, const auto& b) {
                return a.diff.totalTime > b.diff.totalTime;
            });
        // 截取需要的部分 （偏移量） 到 （偏移量 + limit - 1） pagesize 默认是10条
        int64_t pageSize = reqParams.pageSize == 0 ? 10 : reqParams.pageSize;
        int64_t current = reqParams.current;
        uint64_t offset = pageSize * (current - 1);
        if (offset >= cmpRes.size()) {
            offset = cmpRes.size() -
                     ((cmpRes.size() %  pageSize) == 0 ? pageSize : (cmpRes.size() %  pageSize));
        }
        std::vector<Protocol::OperatorStatisticCmpInfoRes>::const_iterator start = cmpRes.begin() + offset;
        std::vector<Protocol::OperatorStatisticCmpInfoRes>::const_iterator end = cmpRes.begin() +
            std::min(offset + pageSize - 1, static_cast<uint64_t>(cmpRes.size() - 1));
        std::vector<Protocol::OperatorStatisticCmpInfoRes> result;
        result.assign(start, end);
        return result;
    }
}
