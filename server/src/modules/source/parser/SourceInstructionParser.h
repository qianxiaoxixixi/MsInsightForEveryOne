/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_SOURCEINSTRUCTIONPARSER_H
#define PROFILER_SERVER_SOURCEINSTRUCTIONPARSER_H

#include "SourceProtocol.h"

namespace Dic::Module::Source {
struct SourceFileLine {
    int line;
    std::vector<float> cycles;
    std::vector<int> instructionsExecuted;
    std::vector<std::pair<std::string, std::string>> addressRange;
};

class SourceInstructionParser {
public:
    SourceInstructionParser() = default;
    bool ConvertToData(std::string &filePath, std::vector<Position> &sourceFilePos, std::vector<Position> &apiFilePos,
                       std::vector<Position> &apiInstrPosArray);
    std::vector<std::string> GetCoreList();
    std::vector<std::string> GetSourceList();
    std::vector<SourceFileLine> GetApiLinesByCoreAndSource(const std::string &core, const std::string &sourceName);
    std::string GetInstr(std::string &filePath);
    std::string GetSourceByName(std::string &sourceName, std::string &filePath);
    void Reset();

private:
    const static uint16_t filePathLengthConst = 4096;
    const static uint16_t addressRangeSize = 2;
    std::map<std::string, Position> sourceFiles;
    std::map<std::string, std::vector<SourceFileLine>> apiFiles;
    std::vector<std::string> apiCores;
    Position apiInstrPos{};

    void ConvertApiInstr(const std::string &jsonStr);
    void ConvertApiFile(const std::string &jsonStr);
    std::map<std::string, std::vector<SourceFileLine>> ConvertToFileMap(rapidjson::Value &fileArray);
    std::vector<SourceFileLine> ConvertToLineArray(rapidjson::Value &lineArray);
};

}

#endif // PROFILER_SERVER_SOURCEINSTRUCTIONPARSER_H
