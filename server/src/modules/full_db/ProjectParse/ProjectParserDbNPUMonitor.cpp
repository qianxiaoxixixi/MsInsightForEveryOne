/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "ProjectParserDbNPUMonitor.h"
#include "CommonDefs.h"
#include "ServerLog.h"
#include "DataBaseManager.h"
#include "ProjectAnalyze.h"

namespace Dic {
namespace Module {
using namespace Server;
using namespace FullDb;

ProjectTypeEnum ProjectParserDbNPUMonitor::GetProjectType(const std::string &dataPath)
{
    return ProjectTypeEnum::DB_NPUMONITOR;
}

std::vector<std::string> ProjectParserDbNPUMonitor::GetParseFileByImportFile(const std::string &importFile,
                                                                             std::string &error)
{
    std::vector<std::string> npuMonitorFiles = FileUtil::FindNPUMonitorFiles(importFile);
    if (npuMonitorFiles.empty()) {
        error = "No parsable db files found, Possible reasons:; 1.File not exist; "
                "2.The nesting depth of the imported sub-file exceeds 5; 3.The sub-file path length exceeds " +
                std::to_string(FileUtil::GetFilePathLengthLimit());
        ServerLog::Info(error);
        return {importFile};
    }
    // npumonitor的表结构和PTA的db格式相同
    DataBaseManager::Instance().SetFileType(FileType::PYTORCH);
    return npuMonitorFiles;
}

void ProjectParserDbNPUMonitor::BuildProjectExploreInfo(Dic::Module::Global::ProjectExplorerInfo &info,
                                                        const std::vector<std::string> &parsedFiles)
{
    ProjectParserBase::BuildProjectExploreInfo(info, parsedFiles);
    for (const auto &parsedFile : parsedFiles) {
        auto parseFileInfoRank = std::make_shared<ParseFileInfo>();
        parseFileInfoRank->parseFilePath = parsedFile;
        parseFileInfoRank->type = ParseFileType::RANK;
        parseFileInfoRank->subId = parsedFile;
        parseFileInfoRank->curDirName = FileUtil::GetFileName(parsedFile);
        parseFileInfoRank->fileId = parsedFile;
        info.AddSubParseFileInfo(info.fileName, ParseFileType::PROJECT, parseFileInfoRank);
    }
}

ProjectAnalyzeRegister<ProjectParserDbNPUMonitor> pRegDBNPUMonitor(ParserType::DB_NPUMONITOR);
}
}
