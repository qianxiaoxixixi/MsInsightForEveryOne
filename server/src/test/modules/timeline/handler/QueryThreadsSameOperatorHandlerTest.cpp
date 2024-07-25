/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryThreadsSameOperatorHandler.h"
#include "HandlerTest.cpp"

class QueryThreadsSameOperatorHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryThreadsSameOperatorHandlerTestNormal)
{
    Dic::Module::Timeline::QueryThreadsSameOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitThreadsOperatorsRequest>();
    handler.HandleRequest(std::move(requestPtr));
}

