/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 */

#ifndef DIC_SOURCE_PROTOCOL_SOURCE_RESPONSE_H
#define DIC_SOURCE_PROTOCOL_SOURCE_RESPONSE_H

#include <vector>
#include "GlobalDefs.h"
#include "ProtocolDefs.h"
#include "ProtocolMessage.h"

namespace Dic {
namespace Protocol {
struct SourceCodeFileResBody {
    std::string fileContent;
};

struct SourceCodeFileResponse : public Response {
    SourceCodeFileResponse() : Response(REQ_RES_SOURCE_CODE_FILE) {}

    SourceCodeFileResBody body;
};

struct SourceFileLineRes {
    int line;
    float cycle;
    int instructionExecuted;
    std::vector<std::pair<std::string, std::string>> addressRange;
};

struct SourceApiLineResBody {
    std::vector<SourceFileLineRes> lines;
};

struct SourceApiLineResponse : public Response {
    SourceApiLineResponse() : Response(REQ_RES_SOURCE_API_LINE) {}

    SourceApiLineResBody body;
};

struct SourceApiInstrResBody {
    std::string instructions;
};

struct SourceApiInstrResponse : public Response {
    SourceApiInstrResponse() : Response(REQ_RES_SOURCE_API_INSTRUCTIONS) {}

    SourceApiInstrResBody body;
};

struct BlockDetailBody {
    int64_t blockId;
    std::string coreType;
    std::vector<std::string> duration;
};

struct DetailsBaseInfoResBody {
    std::string name;
    std::string soc;
    std::string opType;
    int64_t blockDim;
    int64_t mixBlockDim;
    std::string duration;
    std::vector<BlockDetailBody> blockDetail;
    std::vector<std::string> advice;
};

struct DetailsBaseInfoResponse : public Response {
    DetailsBaseInfoResponse() : Response(REQ_RES_DETAILS_BASE_INFO) {}
    DetailsBaseInfoResBody body;
};

struct SubBlockUnitData {
    std::int64_t blockId;
    std::string blockType;
    std::string name;
    std::string unit;
    std::string value;
    std::string originValue;
};

struct SubBlockData {
    std::vector<SubBlockUnitData> detailDataList;
    std::vector<std::string> advice;
};

struct DetailsLoadInfoResBody {
    std::vector<int64_t> blockIdList;
    SubBlockData chartData;
    SubBlockData tableData;
};

struct DetailsLoadInfoResponse : public Response {
    DetailsLoadInfoResponse() : Response(REQ_RES_DETAILS_COMPUTE_LOAD_INFO) {}
    DetailsLoadInfoResBody body;
};
} // end of namespace Protocol
} // end of namespace Dic

#endif // DIC_SOURCE_PROTOCOL_SOURCE_RESPONSE_H
