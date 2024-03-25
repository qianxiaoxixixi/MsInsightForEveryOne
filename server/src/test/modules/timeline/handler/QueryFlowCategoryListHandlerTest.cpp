/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryFlowCategoryListHandler.h"
#include "HandlerTest.cpp"

class QueryFlowCategoryListHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryFlowCategoryListHandlerTestNormal)
{
    Dic::Module::Timeline::QueryFlowCategoryListHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::FlowCategoryListRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh";
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QueryFlowCategoryListHandlerTestTokenIsError)
{
    Dic::Module::Timeline::QueryFlowCategoryListHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::FlowCategoryListRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh3";
    handler.HandleRequest(std::move(requestPtr));
}