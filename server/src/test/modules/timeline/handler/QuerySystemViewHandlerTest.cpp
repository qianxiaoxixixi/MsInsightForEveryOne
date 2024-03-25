/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QuerySystemViewHandler.h"
#include "HandlerTest.cpp"

class QuerySystemViewHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QuerySystemViewHandlerTestNormal)
{
    Dic::Module::Timeline::QuerySystemViewHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::SystemViewRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh";
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QuerySystemViewHandlerTestTokenIsError)
{
    Dic::Module::Timeline::QuerySystemViewHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::SystemViewRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh3";
    handler.HandleRequest(std::move(requestPtr));
}
