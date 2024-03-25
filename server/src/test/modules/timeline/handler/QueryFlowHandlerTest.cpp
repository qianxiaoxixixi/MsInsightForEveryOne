/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryFlowHandler.h"
#include "HandlerTest.cpp"

class QueryFlowHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryFlowHandlerTestNormal)
{
    Dic::Module::Timeline::QueryFlowHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitFlowRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh";
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QueryFlowHandlerTestTokenIsError)
{
    Dic::Module::Timeline::QueryFlowHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitFlowRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh3";
    handler.HandleRequest(std::move(requestPtr));
}