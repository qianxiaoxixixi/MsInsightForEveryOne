/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "AdvisorProcessUtil.h"
#include "DataBaseManager.h"
#include "ServerLog.h"
#include "TraceTime.h"
#include "VirtualTraceDatabase.h"
#include "AffinityAPIAdvisor.h"

namespace Dic::Module::Advisor {
using namespace Dic::Server;
bool AffinityAPIAdvisor::Process(const Protocol::APITypeParams &params, Protocol::AffinityAPIResBody &resBody)
{
    auto database = Timeline::DataBaseManager::Instance().GetTraceDatabase(params.rankId);
    if (database == nullptr) {
        ServerLog::Error("Failed to get connection for Affinity API query. fileId:", params.rankId);
        return false;
    }

    uint64_t startTime = Timeline::TraceTime::Instance().GetStartTime();
    Protocol::KernelDetailsParams param = {.orderBy = params.orderBy, .order = params.orderType,
                                           .current = params.currentPage, .pageSize = params.pageSize};
    param.order = params.orderType == "ascend" ? "ASC" : "DESC";
    if (std::count(AFFINITY_API_ORDER_BY_NAME_LIST.begin(),
                   AFFINITY_API_ORDER_BY_NAME_LIST.end(), params.orderBy) == 0) {
        param.orderBy = "duration";
    }
    std::map<uint64_t, std::vector<Protocol::FlowLocation>> dataMap{};
    std::map<uint64_t, std::vector<uint32_t>> indexMap{};
    if (!database->QueryAffinityAPIData(param, GetFirstApiList(AFFINITY_API_RULE), startTime, dataMap, indexMap)) {
        ServerLog::Error("Failed to Query Affinity API from database.");
        return false;
    }
    std::vector<Protocol::FlowLocation> results;
    for (const auto& it : dataMap) { // 获取某个泳道的数据
        uint64_t trackId = it.first;
        std::vector<Protocol::FlowLocation> datas = it.second;
        std::vector<uint32_t> indexList = indexMap[trackId];
        FilterAffinityApiData(params, datas, indexList, results);
    }
    AdvisorProcessUtil::SortFlowLocationData(results, param);
    uint64_t start = param.pageSize * (param.current - 1);
    for (uint64_t i = start; i < start + param.pageSize && i < results.size(); ++i) {
        auto item = results.at(i);
        Protocol::AffinityAPIData one{};
        one.name = item.name;
        one.baseInfo.pid = item.pid;
        one.baseInfo.tid = item.tid;
        one.baseInfo.startTime = item.timestamp;
        one.baseInfo.duration = item.duration;
        one.originAPI = item.type;
        one.replaceAPI = item.metaType;
        one.note = item.deviceId;
        resBody.datas.emplace_back(one);
    }
    resBody.size = results.size();
    return true;
}

std::vector<std::string> AffinityAPIAdvisor::GetFirstApiList(const std::vector<AffinityApiData> &affinityApiData)
{
    std::vector<std::string> apiList{};
    if (affinityApiData.empty()) {
        return apiList;
    }
    for (const auto& item : affinityApiData) {
        if (item.apiList.empty()) {
            continue;
        }
        if (std::count(apiList.begin(), apiList.end(), item.apiList[0]) != 0) {
            continue;
        }
        apiList.emplace_back(item.apiList[0]);
    }
    return apiList;
}

// 匹配连续的api
void AffinityAPIAdvisor::FilterAffinityApiData(const Protocol::APITypeParams &params,
    std::vector<Protocol::FlowLocation> &dataList, const std::vector<uint32_t> &indexList,
    std::vector<Protocol::FlowLocation> &result)
{
    if (dataList.empty() || indexList.empty()) {
        return;
    }

    for (auto index : indexList) { //
        for (const auto& rule : AFFINITY_API_RULE) {
            Protocol::AffinityAPIData one{};
            if (!CheckApiSeqWithRule(rule.apiList, dataList, index)) {
                continue;
            }
            dataList[index].type = StringUtil::join(rule.apiList, ",");
            dataList[index].metaType = rule.affinityApi;
            dataList[index].deviceId = rule.note;
            result.emplace_back(dataList[index]);
        }
    }
}

bool AffinityAPIAdvisor::CheckApiSeqWithRule(const std::vector<std::string> &rule,
    const std::vector<Protocol::FlowLocation> &dataList, uint32_t index)
{
    std::string name = dataList[index].name;
    if (name.substr(name.find(API_SEP) + API_SEP.length(), name.length()) != rule[0]) {
        return false; // 匹配rule中第一个API，不匹配规则时跳过
    }
    if (index + rule.size() >= dataList.size()) {
        return false;  // 真实数据长度 < 预期数据长度，无法匹配
    }

    for (int i = 1; i < rule.size(); ++i) { // 上文已匹配索引为0的数据
        std::string tmp = dataList[index + i].name;
        if (tmp.substr(tmp.find(API_SEP) + API_SEP.length(), tmp.length()) != rule[i]) {
            return false; // 不完全匹配，则跳过
        }
    }

    return true;
}
} // Dic::Module::Advisor