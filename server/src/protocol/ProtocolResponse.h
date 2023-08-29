/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Protocol Response declaration
 */

#ifndef DIC_PROTOCOL_RESPONSE_H
#define DIC_PROTOCOL_RESPONSE_H

#include <vector>
#include "GlobalDefs.h"
#include "ProtocolDefs.h"
#include "ProtocolEntity.h"
#include "ProtocolBase.h"

namespace Dic {
namespace Protocol {
#pragma region << Global>>

// token create
// token.create can be sent to master session and slave sessions.
struct TokenCreateResBody {
    uint32_t createTime = 0; // UTC
    std::optional<std::string> parentToken;
};

struct TokenCreateResponse : public Response {
    TokenCreateResponse() : Response(REQ_RES_TOKEN_CREATE) {};
    TokenCreateResBody body;
};

// token.destroy
struct TokenDestroyResBody {
    uint32_t destroyTime = 0; // UTC
    std::string destroyToken;
};

struct TokenDestroyResponse : public Response {
    TokenDestroyResponse() : Response(REQ_RES_TOKEN_DESTROY) {};
    TokenDestroyResBody body;
};

// token.check
struct TokenCheckResBody {
    uint32_t createTime = 0;
    uint32_t deadTime = 0;
    std::string checkedToken;
    bool isSubToken = false;
};

struct TokenCheckResponse : public Response {
    TokenCheckResponse() : Response(REQ_RES_TOKEN_CHECK) {};
    TokenCheckResBody body;
};

// config.get
struct ConfigGetResBody {
    std::optional<GlobalConfig> globalConfig;
};

struct ConfigGetResponse : public Response {
    ConfigGetResponse() : Response(REQ_RES_CONFIG_GET) {};
    ConfigGetResBody body;
};

// config.set
struct ConfigSetResBody {
    uint32_t configSetTime = 0;
    bool isAlertMsg = false;
};

struct ConfigSetResponse : public Response {
    ConfigSetResponse() : Response(REQ_RES_CONFIG_SET) {};
    ConfigSetResBody body;
};

#pragma endregion

#pragma region <<timeline>>

struct Action {
    std::string cardName;
    std::string rankId;
    bool result = true;
};

struct ImportActionResBody {
    std::vector<Action> result;
};

struct ImportActionResponse : public Response {
    ImportActionResponse() : Response(REQ_RES_IMPORT_ACTION) {}
    ImportActionResBody body;
};

struct ThreadTraces {
    std::string name;
    uint64_t duration = 0;
    uint64_t startTime = 0;
    uint64_t endTime = 0;
    int32_t depth = 0;
    int32_t threadId = 0;
};

struct UnitThreadTracesBody {
    std::vector<std::vector<ThreadTraces>> data;
};

struct UnitThreadTracesResponse : public Response {
    UnitThreadTracesResponse() : Response(REQ_RES_UNIT_THREAD_TRACES) {}
    UnitThreadTracesBody body;
};

struct Threads {
    std::string title;
    uint64_t wallDuration = 0;
    uint64_t occurrences = 0;
    uint64_t avgWallDuration = 0;
    uint64_t selfTime = 0;
};

struct UnitThreadsBody {
    bool emptyFlag = false;
    std::vector<Threads> data;
};

struct UnitThreadsResponse : public Response {
    UnitThreadsResponse() : Response(REQ_RES_UNIT_THREADS) {}
    UnitThreadsBody body;
};

struct ThreadDetail {
    uint64_t selfTime = 0;
    uint64_t duration = 0;
    std::string args;
    std::string title;
    std::string cat;
};

struct UnitThreadDetailBody {
    bool emptyFlag = false;
    ThreadDetail data;
};

struct UnitThreadDetailResponse : public Response {
    UnitThreadDetailResponse() : Response(REQ_RES_UNIT_THREAD_DETAIL) {}
    UnitThreadDetailBody body;
};

struct FlowName {
    int32_t depth = 0;
    int32_t tid = 0;
    uint64_t timestamp = 0;
    std::string title;
    std::string pid;
    std::string flowId;
};

struct UnitFlowNameBody {
    std::vector<FlowName> flowDetail;
};

struct UnitFlowNameResponse : public Response {
    UnitFlowNameResponse() : Response(REQ_RES_UNIT_FLOW_NAME) {}
    UnitFlowNameBody body;
};

struct FlowLocation {
    int32_t tid = 0;
    int32_t depth = 0;
    uint64_t timestamp = 0;
    std::string pid;
};

struct UnitFlowBody {
    std::string title;
    std::string cat;
    std::string id;
    FlowLocation from;
    FlowLocation to;
};

struct UnitFlowResponse : public Response {
    UnitFlowResponse() : Response(REQ_RES_UNIT_FLOW) {}
    UnitFlowBody body;
};

struct ResetWindowResponse : public Response {
    ResetWindowResponse() : Response(REQ_RES_RESET_WINDOW) {}
};

struct Chart {
    uint32_t ts = 0;
    uint32_t value = 0;
};

struct UnitChartBody {
    std::vector<Chart> data;
};

struct UnitChartResponse : public Response {
    UnitChartResponse() : Response(REQ_RES_UNIT_CHART) {}
    UnitChartBody body;
};

// struct
struct RowThreadTrace {
    int64_t id = 0;
    uint64_t start_time = 0;
    uint64_t duration = 0;
    int32_t depth = 0;
    int64_t trace_id = 0;
    std::string name;
};

struct ExtremumTimestamp {
    uint64_t minTimestamp = 0;
    uint64_t maxTimestamp = 0;
};

struct SimpleSlice {
    uint64_t timestamp = 0;
    uint64_t duration = 0;
    uint64_t endTime = 0;
    int32_t depth = 0;
    std::string name;
};

struct SliceDto {
    uint64_t id = 0;
    uint64_t timestamp = 0;
    uint64_t duration = 0;
    int32_t depth = 0;
    int64_t track_id = 0;
    std::string name;
    std::string args;
    std::string cat;
};

struct FlowDetailDto {
    std::string name;
    std::string cat;
    std::string flowId;
    std::string pid;
    int32_t tid = 0;
    int32_t depth = 0;
    uint64_t timestamp = 0;
    std::string type;
};

struct SimpleFlowDto {
    std::string name;
    std::string flowId;
    std::string type;
};

struct SliceFlowDetail {
    int32_t tid = 0;
    int32_t depth = 0;
    uint64_t timestamp = 0;
    std::string pid;
};
#pragma endregion
} // end of namespace Protocol
} // end of namespace Dic

#endif // DIC_PROTOCOL_RESPONSE_H
