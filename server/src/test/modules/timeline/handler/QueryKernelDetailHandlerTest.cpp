/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryKernelDetailHandler.h"
#include "HandlerTest.cpp"

class QueryKernelDetailHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryKernelDetailHandlerTestNormal)
{
    Dic::Module::Timeline::QueryKernelDetailHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::KernelDetailsRequest>();
    handler.HandleRequest(std::move(requestPtr));
}
