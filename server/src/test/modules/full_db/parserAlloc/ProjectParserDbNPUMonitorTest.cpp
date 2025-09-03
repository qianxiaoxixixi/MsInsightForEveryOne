/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include "DataBaseManager.h"
#include "ProjectParserDbNPUMonitor.h"

using namespace Dic;
using namespace Dic::Module;

class ProjectParserDbNPUMonitorTest : public testing::Test {
};

TEST_F(ProjectParserDbNPUMonitorTest, GetProjectTypeTest)
{
    ProjectParserDbNPUMonitor parser;
    std::string path = "/home/Data/npumonitor";
    ProjectTypeEnum type = parser.GetProjectType(path);
    EXPECT_EQ(type, ProjectTypeEnum::DB_NPUMONITOR);
}

TEST_F(ProjectParserDbNPUMonitorTest, GetParseFileByImportFileTestEmptyFolder)
{
#ifdef __linux__
    std::string currPath = Dic::FileUtil::GetCurrPath();
    int index = currPath.find("server");
    const std::string folderPath = currPath.substr(0, index) + "test/data/npumonitor";
    const std::string mkdirCommand = "mkdir -p " + folderPath;
    system(mkdirCommand.c_str());

    ProjectParserDbNPUMonitor parser;
    std::string error;
    std::vector<std::string> npuMonitorFiles = parser.GetParseFileByImportFile(folderPath, error);
    EXPECT_FALSE(error.empty());
    ASSERT_EQ(npuMonitorFiles.size(), 1);
    EXPECT_EQ(npuMonitorFiles[0], folderPath);

    Timeline::DataBaseManager::Instance().Clear();
    const std::string rmCommand = "rm -rf " + currPath.substr(0, index) + "test/data/npumonitor";
    system(rmCommand.c_str());
#endif
}

TEST_F(ProjectParserDbNPUMonitorTest, GetParseFileByImportFileTestValidData)
{
#ifdef __linux__
    std::string currPath = Dic::FileUtil::GetCurrPath();
    int index = currPath.find("server");
    const std::string folderPath = currPath.substr(0, index) + "test/data/npumonitor";
    const std::string dbPath = folderPath + "/msmonitor_99092_20250901114924883_0.db";
    const std::string mkdirCommand = "mkdir -p " + folderPath;
    system(mkdirCommand.c_str());
    const std::string touchCommand = "touch " + dbPath;
    system(touchCommand.c_str());

    ProjectParserDbNPUMonitor parser;
    std::string error;
    std::vector<std::string> npuMonitorFiles = parser.GetParseFileByImportFile(folderPath, error);
    EXPECT_TRUE(error.empty());
    ASSERT_EQ(npuMonitorFiles.size(), 1);
    EXPECT_EQ(npuMonitorFiles[0], dbPath);

    Timeline::DataBaseManager::Instance().Clear();
    const std::string rmCommand = "rm -rf " + currPath.substr(0, index) + "test/data/npumonitor";
    system(rmCommand.c_str());
#endif
}

TEST_F(ProjectParserDbNPUMonitorTest, BuildProjectExploreInfoTest)
{
    ProjectExplorerInfo info;
    info.fileName = "/home/Data/npumonitor";
    std::vector<std::string> parsedFiles = {"/home/Data/npumonitor/msmonitor_99092_20250901114924883_0.db",
        "/home/Data/npumonitor/msmonitor_99093_20250901114924876_1.db"};
    ProjectParserDbNPUMonitor parser;
    parser.BuildProjectExploreInfo(info, parsedFiles);
    ASSERT_EQ(info.subParseFileInfo.size(), 2); // 2
    EXPECT_EQ(info.subParseFileInfo[0]->fileId, "/home/Data/npumonitor/msmonitor_99092_20250901114924883_0.db");
    EXPECT_EQ(info.subParseFileInfo[1]->fileId, "/home/Data/npumonitor/msmonitor_99093_20250901114924876_1.db");
}