/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "InterCoreLoadGraphParser.h"
#include "JsonUtil.h"

namespace Dic {
namespace Module {
namespace Source {
using namespace Dic::Protocol;
using namespace Dic::Server;

bool InterCoreLoadGraphParser::GetInterCoreLoadAnalysisInfo(const std::string &json,
                                                            Protocol::DetailsInterCoreLoadGraphBody &body)
{
    std::optional<InterCoreLoadAnalysisDetail> analysisDetailOpt = ParseInterCoreLoadAnalysisInfo(json);
    if (!analysisDetailOpt.has_value()) {
        return false;
    }
    InterCoreLoadAnalysisDetail &analysisDetail = analysisDetailOpt.value();
    body.advice = analysisDetail.advice;
    body.opType = analysisDetail.opType;
    body.soc = analysisDetail.soc;
    for (const auto &opDetail: analysisDetail.opDetails) {
        DetailsInterCoreLoadOpDetail bodyOpDetail;
        bodyOpDetail.coreId = opDetail.coreId;
        for (const auto &subCoreDetail: opDetail.subCoreDetails) {
            DetailsInterCoreLoadSubCoreDetail bodySubCoreDetail;
            bodySubCoreDetail.SetCyclesDimension(subCoreDetail.cycles, analysisDetail.minCycle);
            bodySubCoreDetail.SetThroughputDimension(subCoreDetail.throughput, analysisDetail.minThroughput);
            bodySubCoreDetail.SetCacheHitRateDimension(subCoreDetail.hitRate, analysisDetail.maxHitRate);
            bodySubCoreDetail.SetSubCoreName(subCoreDetail.subCoreType, subCoreDetail.subCoreIndex);
            bodyOpDetail.AddSubCoreDetail(std::move(bodySubCoreDetail));
        }
        body.AddOpDetail(std::move(bodyOpDetail));
    }
    
    return true;
}

std::optional<InterCoreLoadAnalysisDetail> InterCoreLoadGraphParser::ParseInterCoreLoadAnalysisInfo(
    const std::string &json)
{
    // 解析json并统计每个维度指标的最优值
    InterCoreLoadAnalysisDetail analysisDetail;
    if (json.empty()) {
        return std::nullopt;
    }
    try {
        std::string errorStr;
        const std::optional<document_t> &jsonInfo = JsonUtil::TryParse<kParseNumbersAsStringsFlag>(json, errorStr);
        if (!errorStr.empty() || !jsonInfo.has_value()) {
            ServerLog::Error("Parse inter core load analysis info failed, error is ", errorStr);
            return std::nullopt;
        }
        const document_t &jsonInfoDoc = jsonInfo.value();
        analysisDetail.soc = JsonUtil::GetString(jsonInfoDoc, "soc");
        analysisDetail.opType = JsonUtil::GetString(jsonInfoDoc, "op_type");
        analysisDetail.advice = JsonUtil::GetString(jsonInfoDoc, "advice");

        // 解析op detail数组
        if (!JsonUtil::IsJsonArray(jsonInfoDoc, "op_detail")) {
            ServerLog::Warn("Op detail is not an array.");
            return std::nullopt;
        }
        const json_t &jsonOpDetailArray = jsonInfoDoc["op_detail"];
        for (auto &jsonOpDetail: jsonOpDetailArray.GetArray()) {
            // 解析op detail
            InterCoreOpDetail opDetail;
            opDetail.coreId = JsonUtil::GetInteger(jsonOpDetail, "core_id");

            // 解析sub core detail数组
            if (!JsonUtil::IsJsonArray(jsonOpDetail, "core_detail")) {
                ServerLog::Warn("Core detail is not an array.");
                analysisDetail.AddOpDetail(std::move(opDetail));
                analysisDetail.opDetails.emplace_back(opDetail);
                analysisDetail.opDetails.emplace_back(std::move(opDetail));
                continue;
            }
            const json_t &jsonCoreDetailArray = jsonOpDetail["core_detail"];
            for (const auto &jsonCoreDetail: jsonCoreDetailArray.GetArray()) {
                // 解析sub core detail
                InterCoreSubCoreDetail subCoreDetail;
                subCoreDetail.subCoreIndex = JsonUtil::GetInteger(jsonCoreDetail, "subcore_id");
                subCoreDetail.subCoreType = JsonUtil::GetString(jsonCoreDetail, "subcore_type");
                subCoreDetail.cycles = JsonUtil::GetInteger(jsonCoreDetail, "cycles");
                subCoreDetail.throughput = JsonUtil::GetFloat(jsonCoreDetail, "throughput");
                subCoreDetail.hitRate = JsonUtil::GetFloat(jsonCoreDetail, "L2cache_hit_rate");

                // 设置最优值
                analysisDetail.SetMaxHitRate(subCoreDetail.hitRate);
                analysisDetail.SetMinThroughput(subCoreDetail.throughput);
                analysisDetail.SetMinCycle(subCoreDetail.cycles);
                opDetail.subCoreDetails.emplace_back(subCoreDetail);
            }
            analysisDetail.AddOpDetail(std::move(opDetail));
        }
    } catch (const std::exception &e) {
        ServerLog::Error("Parse inter core load analysis info failed, exception is ", e.what());
        return std::nullopt;
    }
    
    return {std::move(analysisDetail)};
}

} // Dic
} // Module
} // Source