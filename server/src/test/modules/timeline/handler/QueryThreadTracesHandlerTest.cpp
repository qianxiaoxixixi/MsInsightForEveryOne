/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryThreadTracesHandler.h"
#include "HandlerTest.cpp"

class QueryThreadTracesHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryThreadTracesHandlerTestNormal)
{
    Dic::Module::Timeline::QueryThreadTracesHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitThreadTracesRequest>();
    handler.HandleRequest(std::move(requestPtr));
}
