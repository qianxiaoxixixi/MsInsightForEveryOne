/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 */

#include "ServerLog.h"
#include "JsonUtil.h"
#include "ProtocolEnumUtil.h"
#include "SourceProtocol.h"
#include "SourceProtocolUtil.h"

namespace Dic {
namespace Protocol {
using namespace Dic::Server;
using namespace rapidjson;
#pragma region <<Response to json>>

template <typename RESPONSE> std::optional<document_t> ToResponseJson(const RESPONSE &response)
{
    ServerLog::Warn("ToResponseJson is not implemented. command:", response.command);
    return std::nullopt;
}

template<> std::optional<document_t> ToResponseJson<SourceCodeFileResponse>(const SourceCodeFileResponse &response)
{
    document_t json(kObjectType);
    auto &allocator = json.GetAllocator();
    ProtocolUtil::SetResponseJsonBaseInfo(response, json);
    json_t body(kObjectType);
    JsonUtil::AddMember(body, "fileContent", response.body.fileContent, allocator);
    JsonUtil::AddMember(json, "body", body, allocator);
    return std::move(json);
}

template<> std::optional<document_t> ToResponseJson<SourceApiLineResponse>(const SourceApiLineResponse &response)
{
    document_t json(kObjectType);
    auto &allocator = json.GetAllocator();
    ProtocolUtil::SetResponseJsonBaseInfo(response, json);
    json_t body(kObjectType);

    json_t lines(kArrayType);
    for (auto lineRes: response.body.lines) {
        json_t line(kObjectType);
        JsonUtil::AddMember(line, "Line", lineRes.line, allocator);
        JsonUtil::AddMember(line, "Instruction Executed", lineRes.instructionExecuted, allocator);
        JsonUtil::AddMember(line, "Cycle", lineRes.cycle, allocator);
        json_t ranges(kArrayType);
        for (auto pair: lineRes.addressRange) {
            json_t range(kArrayType);
            range.PushBack(json_t().SetString(pair.first.c_str(), allocator), allocator);
            range.PushBack(json_t().SetString(pair.second.c_str(), allocator), allocator);
            ranges.PushBack(range, allocator);
        }
        JsonUtil::AddMember(line, "Address Range", ranges, allocator);
        lines.PushBack(line, allocator);
    }
    JsonUtil::AddMember(body, "lines", lines, allocator);
    JsonUtil::AddMember(json, "body", body, allocator);
    return std::move(json);
}

template<> std::optional<document_t> ToResponseJson<SourceApiInstrResponse>(const SourceApiInstrResponse &response)
{
    document_t json(kObjectType);
    auto &allocator = json.GetAllocator();
    ProtocolUtil::SetResponseJsonBaseInfo(response, json);
    json_t body(kObjectType);
    JsonUtil::AddMember(body, "instructions", response.body.instructions, allocator);
    JsonUtil::AddMember(json, "body", body, allocator);
    return std::move(json);
}

template<> std::optional<document_t> ToResponseJson<DetailsBaseInfoResponse>(const DetailsBaseInfoResponse &response)
{
    document_t json(kObjectType);
    auto &allocator = json.GetAllocator();
    ProtocolUtil::SetResponseJsonBaseInfo(response, json);
    json_t body(kObjectType);
    JsonUtil::AddMember(body, "name", response.body.name, allocator);
    JsonUtil::AddMember(body, "soc", response.body.soc, allocator);
    JsonUtil::AddMember(body, "opType", response.body.opType, allocator);
    JsonUtil::AddMember(body, "blockDim", std::to_string(response.body.blockDim), allocator);
    JsonUtil::AddMember(body, "mixBlockDim", response.body.mixBlockDim, allocator);
    JsonUtil::AddMember(body, "duration", response.body.duration, allocator);
    json_t blockDetail(kArrayType);
    for (const auto &item: response.body.blockDetail) {
        json_t detail(kObjectType);
        JsonUtil::AddMember(detail, "blockId", item.blockId, allocator);
        JsonUtil::AddMember(detail, "coreType", item.coreType, allocator);
        json_t durationList(kArrayType);
        for (const auto &duration: item.duration) {
            durationList.PushBack(json_t().SetString(duration.c_str(), allocator),
                                  allocator);
        }
        JsonUtil::AddMember(detail, "duration", durationList, allocator);
        blockDetail.PushBack(detail, allocator);
    }
    JsonUtil::AddMember(body, "blockDetail", blockDetail, allocator);
    json_t advice(kArrayType);
    for (const auto &item: response.body.advice) {
        advice.PushBack(json_t().SetString(item.c_str(), allocator), allocator);
    }
    JsonUtil::AddMember(body, "advice", advice, allocator);
    JsonUtil::AddMember(json, "body", body, allocator);
    return std::move(json);
}

template<> std::optional<document_t> ToResponseJson<DetailsLoadInfoResponse>(const DetailsLoadInfoResponse &response)
{
    document_t json(kObjectType);
    auto &allocator = json.GetAllocator();
    ProtocolUtil::SetResponseJsonBaseInfo(response, json);
    json_t body(kObjectType);
    json_t blockIdList(kArrayType);
    for (const auto &item: response.body.blockIdList) {
        blockIdList.PushBack(json_t().SetString(std::to_string(item).c_str(), allocator), allocator);
    }
    JsonUtil::AddMember(body, "blockIdList", blockIdList, allocator);
    json_t chartData(kObjectType);
    json_t detailDataList(kArrayType);
    for (const auto &item: response.body.chartData.detailDataList) {
        json_t unitData(kObjectType);
        JsonUtil::AddMember(unitData, "blockId", std::to_string(item.blockId), allocator);
        JsonUtil::AddMember(unitData, "blockType", item.blockType, allocator);
        JsonUtil::AddMember(unitData, "name", item.name, allocator);
        JsonUtil::AddMember(unitData, "unit", item.unit, allocator);
        JsonUtil::AddMember(unitData, "value", item.value, allocator);
        JsonUtil::AddMember(unitData, "originValue", item.originValue, allocator);
        detailDataList.PushBack(unitData, allocator);
    }
    JsonUtil::AddMember(chartData, "detailDataList", detailDataList, allocator);
    json_t advice(kArrayType);
    for (const auto &item: response.body.chartData.advice) {
        advice.PushBack(json_t().SetString(item.c_str(), allocator), allocator);
    }
    JsonUtil::AddMember(chartData, "advice", advice, allocator);
    JsonUtil::AddMember(body, "chartData", chartData, allocator);
    json_t tableData(kObjectType);
    json_t tableDetailDataList(kArrayType);
    for (const auto &item: response.body.tableData.detailDataList) {
        json_t unitData(kObjectType);
        JsonUtil::AddMember(unitData, "blockId", std::to_string(item.blockId), allocator);
        JsonUtil::AddMember(unitData, "blockType", item.blockType, allocator);
        JsonUtil::AddMember(unitData, "name", item.name, allocator);
        JsonUtil::AddMember(unitData, "unit", item.unit, allocator);
        JsonUtil::AddMember(unitData, "value", item.value, allocator);
        tableDetailDataList.PushBack(unitData, allocator);
    }
    JsonUtil::AddMember(tableData, "detailDataList", tableDetailDataList, allocator);
    json_t tableAdvice(kArrayType);
    for (const auto &item: response.body.tableData.advice) {
        tableAdvice.PushBack(json_t().SetString(item.c_str(), allocator), allocator);
    }
    JsonUtil::AddMember(tableData, "advice", tableAdvice, allocator);
    JsonUtil::AddMember(body, "tableData", tableData, allocator);
    JsonUtil::AddMember(json, "body", body, allocator);
    return std::move(json);
}

#pragma endregion
} // end of namespace Protocol
} // end of namespace Dic