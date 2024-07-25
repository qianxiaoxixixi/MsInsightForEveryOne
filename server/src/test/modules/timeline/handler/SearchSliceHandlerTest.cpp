/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "SearchSliceHandler.h"
#include "HandlerTest.cpp"

class SearchSliceHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, SearchSliceHandlerTestNormal)
{
    Dic::Module::Timeline::SearchSliceHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::SearchSliceRequest>();
    handler.HandleRequest(std::move(requestPtr));
}

