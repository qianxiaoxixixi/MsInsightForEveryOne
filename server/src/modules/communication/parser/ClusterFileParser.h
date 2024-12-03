/*
 *  Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 *
 */

#ifndef PROFILER_SERVER_CLUSTERFILEPARSER_H
#define PROFILER_SERVER_CLUSTERFILEPARSER_H

#include "pch.h"
#include <string>
#include <vector>
#include "VirtualClusterDatabase.h"
#include "ClusterDef.h"

namespace Dic {
namespace Module {
namespace Timeline {
enum class AttDataType {
    TEXT,
    DB
};
class ClusterFileParser {
public:
    bool ParseClusterFiles();
    bool ParseClusterStep2Files();
    bool ParseCommunication(const std::vector<std::string> &filePathList);
    void ParseStepStatisticsFile(const std::vector<std::string> &filePathList);
    void SaveClusterBaseInfo(const std::string& selectedPath);
    void ParseCommunicationMatrix(const std::vector<std::string> &filePathList);
    static void ParseCommunicationGroup(const std::string selectedPath, ClusterBaseInfo &baseInfo);
    bool ParserClusterOfDb();
    std::string GetClusterDbPath();
    ClusterFileParser(const std::string &filePath, std::shared_ptr<VirtualClusterDatabase> &database);
private:
    void SaxParseJsonFile(const std::string& filePath, int saxHandlerType);
    bool InitClusterDatabase();
    StepStatistic MapToStepStatistic(std::map<std::string, size_t> &dataMap,
                                     const std::vector<std::string> &tokens);
    size_t subStrlen = 2;
    std::string clusterDbPath;
    bool needClearDb = true;
    // cluster_step_trace_time.csv文件最小列数需要为11列，否则会造成数组越界
    static const int minStepTraceTimeColumnNumber = 11;
    std::string selectedFilePath;
    std::shared_ptr<VirtualClusterDatabase> database;
    static bool AttAnalyze(const std::string &selectedPath,
                           const std::string &mode,
                           AttDataType dataType = AttDataType::TEXT);
    bool TransCommunicationToDb(const std::string &selectedPath, const std::regex &patternCommunication);
    static bool CheckDocumentValid(const Document &doc);
    static std::string GetStrValue(std::map<std::string, size_t> &dataMap, const std::vector<std::string> &tokens,
                                   const std::string &key);
};
} // end of namespace Timeline
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_CLUSTERFILEPARSER_H
