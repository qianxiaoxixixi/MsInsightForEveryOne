/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryFlowCategoryEventsHandler.h"
#include "HandlerTest.cpp"

class QueryFlowCategoryEventsHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryFlowCategoryEventsHandlerTestPathIsEmpty)
{
    Dic::Module::Timeline::QueryFlowCategoryEventsHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::FlowCategoryEventsRequest>();
    handler.HandleRequest(std::move(requestPtr));
}
