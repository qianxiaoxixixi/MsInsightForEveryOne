/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "ResetWindowHandler.h"
#include "HandlerTest.cpp"

class ResetWindowHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, ResetWindowHandlerTestNormal)
{
    Dic::Module::Timeline::ResetWindowHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::ResetWindowRequest>();
    handler.HandleRequest(std::move(requestPtr));
}

