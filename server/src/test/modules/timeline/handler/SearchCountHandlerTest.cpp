/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "SearchCountHandler.h"
#include "HandlerTest.cpp"

class SearchCountHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, SearchCountHandlerTestNormal)
{
    Dic::Module::Timeline::SearchCountHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::SearchCountRequest>();
    handler.HandleRequest(std::move(requestPtr));
}

