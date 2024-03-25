/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "QueryOneKernelHandler.h"
#include "HandlerTest.cpp"

class QueryOneKernelHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, QueryOneKernelHandlerTestNormal)
{
    Dic::Module::Timeline::QueryOneKernelHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::KernelRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh";
    handler.HandleRequest(std::move(requestPtr));
}

TEST_F(HandlerTest, QueryOneKernelHandlerTestTokenIsError)
{
    Dic::Module::Timeline::QueryOneKernelHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::KernelRequest>();
    requestPtr.get()->token = "hhhhhhhhhhhhh3";
    handler.HandleRequest(std::move(requestPtr));
}