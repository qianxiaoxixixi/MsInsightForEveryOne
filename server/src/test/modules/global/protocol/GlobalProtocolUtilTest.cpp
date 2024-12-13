/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "GlobalProtocolResponse.h"
#include "JsonUtil.h"
#include "GlobalProtocolUtil.h"
class GlobalProtocolUtilTest : public ::testing::Test {};

TEST_F(GlobalProtocolUtilTest, TestTokenHeartCheckResponse)
{
    Dic::Protocol::TokenHeartCheckResponse response;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
}

TEST_F(GlobalProtocolUtilTest, TestFilesGetResponseToJson)
{
    Dic::Protocol::FilesGetResponse response;
    std::unique_ptr<Dic::Protocol::File> file1 = std::make_unique<Dic::Protocol::File>();
    std::unique_ptr<Dic::Protocol::File> file2 = std::make_unique<Dic::Protocol::File>();
    std::unique_ptr<Dic::Protocol::Folder> folder = std::make_unique<Dic::Protocol::Folder>();
    std::unique_ptr<Dic::Protocol::Folder> folder2 = std::make_unique<Dic::Protocol::Folder>();
    folder2->childrenFiles.emplace_back(std::move(file2));
    folder->childrenFolders.emplace_back(std::move(folder2));
    response.body.childrenFiles.emplace_back(std::move(file1));
    response.body.childrenFolders.emplace_back(std::move(folder));
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
}

TEST_F(GlobalProtocolUtilTest, TestProjectExplorerInfoUpdateResponseToJson)
{
    Dic::Protocol::ProjectExplorerInfoUpdateResponse response;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
}

TEST_F(GlobalProtocolUtilTest, TestProjectExplorerInfoGetResponseToJson)
{
    Dic::Protocol::ProjectExplorerInfoGetResponse response;
    Dic::Protocol::ProjectDirectoryInfo projectDirectoryInfo;
    projectDirectoryInfo.fileName.emplace_back("llllllll");
    response.body.projectDirectoryList.emplace_back(projectDirectoryInfo);
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr = "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":false,\"command\":\"files/"
        "getProjectExplorer\",\"moduleName\":\"unknown\",\"body\":{\"projectDirectoryList\":[{"
        "\"projectName\":\"\",\"fileName\":[\"llllllll\"]}]}}";
    EXPECT_EQ(json, jsonStr);
}

TEST_F(GlobalProtocolUtilTest, TestProjectExplorerInfoDeleteResponseToJson)
{
    Dic::Protocol::ProjectExplorerInfoDeleteResponse response;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr = "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":false,\"command\":\"files/"
        "deleteProjectExplorer\",\"moduleName\":\"unknown\",\"body\":{}}";
    EXPECT_EQ(json, jsonStr);
}

TEST_F(GlobalProtocolUtilTest, TestProjectCheckValidResponseToJson)
{
    Dic::Protocol::ProjectCheckValidResponse response;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr = "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":false,\"command\":\"files/"
        "checkProjectValid\",\"moduleName\":\"unknown\",\"body\":{\"result\":0}}";
    EXPECT_EQ(json, jsonStr);
}

TEST_F(GlobalProtocolUtilTest, TestBaselineSettingResponseToJson)
{
    Dic::Protocol::BaselineSettingResponse response;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr = "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":false,\"command\":\"global/"
        "setBaseline\",\"moduleName\":\"unknown\",\"body\":{\"rankId\":\"\",\"host\":\"\","
        "\"cardName\":\"\",\"isCluster\":false,\"errorMessage\":\"\"}}";
    EXPECT_EQ(json, jsonStr);
}

TEST_F(GlobalProtocolUtilTest, TestBaselineCancelResponseToJson)
{
    Dic::Protocol::BaselineCancelResponse response;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr = "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":false,\"command\":\"global/"
        "cancelBaseline\",\"moduleName\":\"unknown\",\"body\":{}}";
    EXPECT_EQ(json, jsonStr);
}
