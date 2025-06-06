/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryCommunicationKernelHandler.h"
#include "HandlerTest.cpp"

class QueryCommunicationKernelHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryCommunicationKernelHandlerTestParamIsEmpty)
{
    Dic::Module::Timeline::QueryCommunicationKernelHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::CommunicationKernelRequest>();
    bool result = handler.HandleRequest(std::move(requestPtr));
    EXPECT_EQ(result, false);
}
