/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "ConstantDefs.h"
#include "JupyterProtocolUtil.h"
#include "JupyterProtocol.h"
#include "JupyterProtocolEvent.h"

using namespace Dic::Protocol;
class JupyterProtocolUtilTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        jupyterProtocol.Register();
    }

    void TearDown() override
    {
        jupyterProtocol.UnRegister();
    }

    Dic::Protocol::JupyterProtocol jupyterProtocol;
};

TEST_F(JupyterProtocolUtilTest, ToParseJupyterCompletedEventJsonTestReturnNormal)
{
    ParseJupyterCompletedEvent event{};
    std::string err;
    event.body.parseResult = Dic::PARSE_RESULT_OK;
    event.body.url = "http://localhost:8080";
    std::optional<Dic::document_t> jsonOptional = jupyterProtocol.ToJson(event, err);
    EXPECT_EQ(jsonOptional.has_value(), true);
    EXPECT_EQ(jsonOptional.value().HasMember("body"), true);
    auto jsonBody = jsonOptional.value()["body"].GetObject();
    EXPECT_EQ(jsonBody.HasMember("parseResult"), true);
    EXPECT_EQ(jsonBody.HasMember("url"), true);
    EXPECT_EQ(jsonBody["parseResult"].GetString(), event.body.parseResult);
    EXPECT_EQ(jsonBody["url"].GetString(), event.body.url);
}