/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "RemoteDeleteHandler.h"
#include "HandlerTest.cpp"

class RemoteDeleteHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, RemoteDeleteHandlerTestNormal)
{
    Dic::Module::Timeline::RemoteDeleteHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::RemoteDeleteRequest>();
    handler.HandleRequest(std::move(requestPtr));
}
