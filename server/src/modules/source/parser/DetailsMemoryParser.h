/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef INSTRUCTION_TIMELINE_PARSER_H
#define INSTRUCTION_TIMELINE_PARSER_H

#include "SourceProtocolResponse.h"
#include "SourceProtocol.h"

namespace Dic::Module::Source {
class DetailsMemoryParser {
public:
    bool GetDetailsMemoryGraph(const std::string& targetBlockId, Protocol::DetailsMemoryGraphResBody& responseBody,
        std::string& curFilePath, std::map<int, std::vector<Position>>& curBlockMap);
    bool GetDetailsMemoryTable(const std::string& targetBlockId, Protocol::DetailsMemoryTableResBody& responseBody,
        std::string& curFilePath, std::map<int, std::vector<Position>>& curBlockMap);
    bool GetDetailsBaseInfo(Protocol::DetailsBaseInfoResBody& responseBody, std::string& curFilePath,
        std::map<int, std::vector<Position>>& curBlockMap);
    bool GetDetailsLoadInfo(Protocol::DetailsLoadInfoResBody& responseBody, std::string& curFilePath,
        std::map<int, std::vector<Position>>& curBlockMap);
protected:
    static Protocol::MemoryGraph ParseJsonToMemoryGraph(const json_t& json);
    static Protocol::MemoryTable ParseJsonToMemoryTable(const json_t& json);
    static Protocol::UtilizationRate ParseJsonToUtilizationRate(const json_t& json);
    static Protocol::DetailsBaseInfoResBody ParseJsonToBaseInfo(const document_t& json);
    static std::string GetUnitType(int64_t unitTypeNumber);
    static Protocol::CompareData<Protocol::SubBlockUnitData> ParseSubBlockUnitData(const json_t &item);
    std::optional<Protocol::SubBlockData> ConvertStrToSubBlockData(const std::string& str);
};

} // Dic
// Module
// Source

#endif // INSTRUCTION_TIMELINE_PARSER_H
