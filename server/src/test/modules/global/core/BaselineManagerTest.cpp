/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "BaselineManager.h"
#include "BaselineManagerService.h"
#include "ProjectExplorerManager.h"
#include "DataBaseManager.h"
#include "ParserStatusManager.h"

using namespace Dic::Module::Global;
class BaselineManagerTest : public ::testing::Test {
public:
    static void SetUpTestSuite()
    {
        std::string systemDbPath = currPath.substr(0, index + 1) + R"(/src/test/test_data/)";
        ProjectExplorerManager::Instance().InitSystemMemoryDbPath(systemDbPath);
        InitProjectExplorerData();
    }

    static void TearDownTestSuite()
    {
        ClearProjectExplorerData();
    }
protected:
    inline static std::string currPath = Dic::FileUtil::GetCurrPath();
    inline static int index = currPath.find_last_of("server");
    inline static int retry = 5;
    static ProjectExplorerInfo CreateProjectData(const std::string &projectName, const std::string &fileName,
                                                 const std::string &importType, Dic::ProjectTypeEnum projectType,
                                                 const std::vector<std::string> parseFileList)
    {
        ProjectExplorerInfo info;
        info.projectName = projectName;
        info.fileName = fileName;
        info.importType = importType;
        info.projectType = static_cast<int64_t>(projectType);
        for (const auto &item: parseFileList) {
            auto parseFileInfo = std::make_shared<ParseFileInfo>();
            parseFileInfo->parseFilePath = item;
            parseFileInfo->subId = item;
            parseFileInfo->type = ParseFileType::RANK;
            info.AddSubParseFileInfo(parseFileInfo);
        }
        return info;
    }

    static ProjectExplorerInfo CreateMultiClusterProject(const std::string &projectName, const std::string &fileName,
                                                         Dic::ProjectTypeEnum projectType, int clusterCount)
    {
        ProjectExplorerInfo info;
        info.projectName = projectName;
        info.fileName = fileName;
        info.importType = "import";
        info.projectType = static_cast<int>(projectType);
        for (int i = 0; i < clusterCount; i++) {
            auto cluster = std::make_shared<ParseFileInfo>();
            std::string clusterName = "cluster_" + std::to_string(i);
            cluster->parseFilePath = clusterName;
            cluster->clusterId = clusterName;
            cluster->type = ParseFileType::CLUSTER;
            cluster->subId = projectName + "/" + clusterName;
            info.AddSubParseFileInfo(cluster);
        }
        return info;
    }

    static void InitProjectExplorerData()
    {
        std::string filePathText = currPath.substr(0, index + 1) +
            R"(/src/test/test_data/test_rank_0/ASCEND_PROFILER_OUTPUT)";
        std::string filePathDb = currPath.substr(0, index + 1) +
            R"(/src/test/test_data/full_db/ascend_pytorch_profiler.db)";
        std::vector<ProjectExplorerInfo> infos;
        std::vector<std::string> parseFileList {filePathText};
        ProjectExplorerInfo info = CreateProjectData("testProject", "projectFilePath",
                                                     "import", Dic::ProjectTypeEnum::TEXT_CLUSTER, parseFileList);
        ProjectExplorerManager::Instance().SaveProjectExplorer(info, false);

        std::vector<std::string> parseDbFileList {filePathDb};
        ProjectExplorerInfo dbInfo = CreateProjectData("testProjectDb", "projectFilePathDb",
                                                       "import", Dic::ProjectTypeEnum::DB, parseDbFileList);
        ProjectExplorerManager::Instance().SaveProjectExplorer(dbInfo, false);
        ProjectExplorerInfo multiClusterInfo = CreateMultiClusterProject("multiCluster", "projectFilePath",
                                                                         Dic::ProjectTypeEnum::DB_CLUSTER,
                                                                         4); // generator 4 cluster
        ProjectExplorerManager::Instance().SaveProjectExplorer(multiClusterInfo, false);
    }

    static void ClearProjectExplorerData()
    {
        ProjectExplorerManager::Instance().DeleteProjectAndFilePath("testProject",
                                                                    std::vector<std::string>());
        ProjectExplorerManager::Instance().DeleteProjectAndFilePath("testProjectDb",
                                                                    std::vector<std::string>());
    }
};

// 测试text数据baseline设置正常情况
TEST_F(BaselineManagerTest, TestText)
{
    std::string filePathText = currPath.substr(0, index + 1) +
        R"(/src/test/test_data/test_rank_0/ASCEND_PROFILER_OUTPUT)";
    BaselineInfo baselineInfo;
    baselineInfo.parsedFilePath = filePathText;
    bool result = BaselineManagerService::InitBaselineData("testProject", filePathText, baselineInfo, COMPARE);
    std::string notFinishTask = "";
    int index = 0;
    while (index < retry && !Dic::Module::Timeline::ParserStatusManager::Instance().IsAllFinished(notFinishTask)) {
        const int sleepTime = 2000;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        index++;
    }
    EXPECT_TRUE(result);
    EXPECT_EQ(BaselineManager::Instance().GetBaselineId(), baselineInfo.rankId);
    Dic::Module::Timeline::DataBaseManager::Instance().Clear();
}

// 测试db数据baseline设置正常情况
TEST_F(BaselineManagerTest, TestDb)
{
    std::string filePathDb = currPath.substr(0, index + 1) +
        R"(/src/test/test_data/full_db/ascend_pytorch_profiler.db)";
    BaselineInfo baselineInfo;
    baselineInfo.parsedFilePath = filePathDb;
    bool result = BaselineManagerService::InitBaselineData("testProjectDb", filePathDb, baselineInfo, COMPARE);
    std::string notFinishTask = "";
    int index = 0;
    while (index < retry && !Dic::Module::Timeline::ParserStatusManager::Instance().IsAllFinished(notFinishTask)) {
        const int sleepTime = 2000;
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        index++;
    }
    EXPECT_TRUE(result);
    EXPECT_EQ(BaselineManager::Instance().GetBaselineId(), baselineInfo.rankId);
    Dic::Module::Timeline::DataBaseManager::Instance().Clear();
}

// 测试db不存在的场景
TEST_F(BaselineManagerTest, TestFileNotExist)
{
    std::string filePathDb = "noData";
    BaselineInfo baselineInfo;
    baselineInfo.parsedFilePath = filePathDb;
    bool result = BaselineManagerService::InitBaselineData("testProjectDb", filePathDb, baselineInfo, COMPARE);
    EXPECT_TRUE(result);
    EXPECT_EQ(baselineInfo.errorMessage, "");
}

TEST_F(BaselineManagerTest, SetGetBaselineClusterPath)
{
    BaselineManager::Instance().SetBaselineClusterPath("baseline");
    EXPECT_EQ(BaselineManager::Instance().GetBaseLineClusterPath(), "baseline");
}

TEST_F(BaselineManagerTest, GetCompareClusterPath)
{
    BaselineManager::Instance().SetCompareClusterPath("compare");
    EXPECT_EQ(BaselineManager::Instance().GetCompareClusterPath(), "compare");
}

TEST_F(BaselineManagerTest, MultiCluster)
{
    std::string filePath = "cluster_2";
    BaselineInfo baselineInfo;
    baselineInfo.parsedFilePath = filePath;
    bool result = BaselineManagerService::InitBaselineData("multiCluster", filePath, baselineInfo, filePath);
    EXPECT_TRUE(result);
    EXPECT_EQ(baselineInfo.isCluster, true);
}