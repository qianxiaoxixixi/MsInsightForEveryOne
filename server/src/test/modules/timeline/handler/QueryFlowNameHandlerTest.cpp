/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryFlowNameHandler.h"
#include "HandlerTest.cpp"

class QueryFlowNameHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryFlowNameHandlerTestNormal)
{
    Dic::Module::Timeline::QueryFlowNameHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitFlowNameRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh";
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QueryFlowNameHandlerTestTokenIsError)
{
    Dic::Module::Timeline::QueryFlowNameHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitFlowNameRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh3";
    handler.HandleRequest(std::move(requestPtr));
}