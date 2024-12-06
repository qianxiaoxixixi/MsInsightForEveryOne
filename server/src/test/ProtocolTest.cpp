/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "gtest/gtest.h"
#include "ProtocolMessageBuffer.h"

using namespace Dic::Protocol;

class ProtocolTest : public testing::Test {
public:
    void SetUp() override
    {
        buffer.Clear();
    }

    ProtocolMessageBuffer buffer;
};

TEST_F(ProtocolTest, MessageWithNoDelim)
{
    std::string req = R"(Content-Length: 9)";
    buffer << req;
    auto message = buffer.Pop();
    EXPECT_EQ(message, nullptr);
}

TEST_F(ProtocolTest, MessgeWithWrongLength)
{
    std::string req = R"(Content-Length: 1024\r\n\r\nhelloworld)";
    buffer << req;
    auto message = buffer.Pop();
    EXPECT_EQ(message, nullptr);
}

TEST_F(ProtocolTest, MessageCannotCastToJson)
{
    std::string req = R"(Content-Length: 10\r\n\r\nhelloworld)";
    buffer << req;
    auto message = buffer.Pop();
    EXPECT_EQ(message, nullptr);
}

TEST_F(ProtocolTest, MessageGetProjectInfo)
{
    std::string req = R"({"id":2, "moduleName":"global", "type":"request", "command":"files/getProjectExplorer", )"
                      R"("projectName":"", "params":{}})";
    buffer << R"(Content-Length: 117\r\n\r\n)";
    buffer << req;
    auto message = buffer.Pop();
    EXPECT_NE(message, nullptr);
    EXPECT_EQ(message->moduleName, "global");
}
