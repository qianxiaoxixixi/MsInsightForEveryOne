/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryThreadTracesSummaryHandler.h"
#include "HandlerTest.cpp"

class QueryThreadTracesSummaryHandlerTest : HandlerTest {};

TEST_F(HandlerTest, QueryThreadTracesSummaryHandlerTestNormal)
{
    Dic::Module::Timeline::QueryThreadTracesSummaryHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr =
        std::make_unique<Dic::Protocol::UnitThreadTracesSummaryRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh";
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QueryThreadTracesSummaryHandlerTokenIsError)
{
    Dic::Module::Timeline::QueryThreadTracesSummaryHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr =
        std::make_unique<Dic::Protocol::UnitThreadTracesSummaryRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh3";
    handler.HandleRequest(std::move(requestPtr));
}
