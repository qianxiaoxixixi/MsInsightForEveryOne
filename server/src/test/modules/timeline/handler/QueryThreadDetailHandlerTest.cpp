/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryThreadDetailHandler.h"
#include "HandlerTest.cpp"

class QueryThreadDetailHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryThreadDetailHandlerTestNormal)
{
    Dic::Module::Timeline::QueryThreadDetailHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::ThreadDetailRequest>();
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QueryThreadDetailHandlerTestIdInvalid)
{
    Dic::Module::Timeline::QueryThreadDetailHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::ThreadDetailRequest>();
    requestPtr->params.id = " where 1=1; DROP TABLES;";
    EXPECT_EQ(handler.HandleRequest(std::move(requestPtr)), false);
}