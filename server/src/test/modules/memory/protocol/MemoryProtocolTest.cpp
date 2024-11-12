/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "MemoryProtocolUtil.h"
#include "MemoryProtocolRequest.h"
#include "MemoryProtocol.h"

using namespace Dic::Protocol;
class MemoryProtocolTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        memoryProtocol.Register();
    }

    void TearDown() override
    {
        memoryProtocol.UnRegister();
    }
    Dic::Protocol::MemoryProtocol memoryProtocol;
};

TEST_F(MemoryProtocolTest, ToMemoryViewRequestTestReturnNormal)
{
    std::string reqJson = R"({"id": 2, "moduleName": "memory", "type": "request", "command": "Memory/view/memoryUsage",
        "resultCallbackId": 0, "params": {"rankId": "1", "type": "Overall", "isCompare": false}})";
    MemoryComponentParams expect = {"1", "Overall", false};
    Dic::document_t json;
    json.Parse(reqJson.c_str());
    std::string err;
    auto result = dynamic_cast<MemoryViewRequest &>(*(memoryProtocol.FromJson(json, err))).params;
    EXPECT_EQ(result.rankId, expect.rankId);
    EXPECT_EQ(result.type, expect.type);
    EXPECT_EQ(result.isCompare, expect.isCompare);
}

TEST_F(MemoryProtocolTest, ToMemoryOperatorSizeRequestTestReturnNormal)
{
    std::string reqJson = R"({"id": 2, "moduleName": "memory", "type": "request", "resultCallbackId": 0,
        "command": "Memory/view/operator/size", "params": {"rankId": "1", "type": "Overall"}})";
    MemoryComponentParams expect = {"1", "Overall", false};
    Dic::document_t json;
    json.Parse(reqJson.c_str());
    std::string err;
    auto result = dynamic_cast<MemoryOperatorSizeRequest &>(*(memoryProtocol.FromJson(json, err))).params;
    EXPECT_EQ(result.rankId, expect.rankId);
    EXPECT_EQ(result.type, expect.type);
    EXPECT_EQ(result.isCompare, expect.isCompare);
}