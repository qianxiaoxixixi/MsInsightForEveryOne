/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryUnitCounterHandler.h"
#include "HandlerTest.cpp"

class QueryUnitCounterHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryUnitCounterHandlerTestNormal)
{
    Dic::Module::Timeline::QueryUnitCounterHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::UnitCounterRequest>();
    handler.HandleRequest(std::move(requestPtr));
}
