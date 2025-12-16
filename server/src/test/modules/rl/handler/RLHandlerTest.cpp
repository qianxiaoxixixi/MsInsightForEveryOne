/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "RLProtocolRequest.h"
#include "RLPipelineHandler.h"
#include "RLModule.h"
#include "../../timeline/handler/HandlerTest.cpp"

using namespace Dic::Module;

// 强化学习流水线接口请求成功
TEST_F(HandlerTest, RLHandlerSuccess)
{
    auto request = std::make_unique<RLPipelineRequest>();
    Dic::Module::RL::RLPipelineHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, true);
}

TEST_F(HandlerTest, RLHandlerOnRequest)
{
    auto module = RLModule();
    auto request = std::make_unique<Request>(std::string_view("unknown"));
    EXPECT_NO_THROW(module.OnRequest(std::move(request)));
}