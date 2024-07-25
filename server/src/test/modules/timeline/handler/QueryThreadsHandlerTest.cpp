/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryThreadsHandler.h"
#include "HandlerTest.cpp"

class QueryThreadsHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryThreadsHandlerTestNormal)
{
    Dic::Module::Timeline::QueryThreadsHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitThreadsRequest>();
    handler.HandleRequest(std::move(requestPtr));
}