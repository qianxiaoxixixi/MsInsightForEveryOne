/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "ProtocolManager.h"
#include "SourceProtocolResponse.h"
#include "SourceProtocolTest.h"

using namespace Dic::Protocol;

class SourceProtocolTest : public ::testing::Test {
protected:
    ProtocolManager *manager;
    std::string error;

protected:
    void SetUp() override
    {
        manager = &ProtocolManager::Instance();
    }
};

TEST_F(SourceProtocolTest, ToCodeFileRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_CODE_FILE_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToApiLineRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_API_LINE_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToApiInstrRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_API_INSTR_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToDetailsBaseInfoRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_BASE_INFO_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToDetailsLoadInfoRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_LOAD_INFO_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToDetailsMemoryGraphRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_MEMORY_GRAPH_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToDetailsMemoryTableRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_MEMORY_TABLE_REQ_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToDetailsInterCoreLoadGraphRequest)
{
    const std::unique_ptr<Request> &ptr = manager->FromJson(TO_INTER_CORE_LOAD_GRAPH_JSON, error);
    EXPECT_EQ(ptr->moduleName, ModuleType::SOURCE);
}

TEST_F(SourceProtocolTest, ToCodeFileResponse)
{
    SourceCodeFileResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToApiLineResponse)
{
    SourceApiLineResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToApiInstrResponse)
{
    SourceApiInstrResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToDetailsBaseInfoResponse)
{
    DetailsBaseInfoResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToDetailsLoadInfoResponse)
{
    DetailsLoadInfoResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToDetailsMemoryGraphResponse)
{
    DetailsMemoryGraphResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToDetailsMemoryTableResponse)
{
    DetailsMemoryTableResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}

TEST_F(SourceProtocolTest, ToDetailsInterCoreLoadGraphResponse)
{
    DetailsInterCoreLoadGraphResponse response;
    response.moduleName = ModuleType::SOURCE;
    manager->ToJson(response, error);
}