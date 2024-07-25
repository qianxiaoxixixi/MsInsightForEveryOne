/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "ImportActionHandler.h"
#include "HandlerTest.cpp"

class ImportActionHandlerTest : HandlerTest {};

TEST_F(HandlerTest, ImportActionHandlerTestPathIsEmpty)
{
    Dic::Module::Timeline::ImportActionHandler importActionHandler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::ImportActionRequest>();
    importActionHandler.HandleRequest(std::move(requestPtr));
}


TEST_F(HandlerTest, ImportActionHandlerTestPathIsNotEmpty)
{
    Dic::Module::Timeline::ImportActionHandler importActionHandler;
    std::unique_ptr<Dic::Protocol::ImportActionRequest> requestPtr =
        std::make_unique<Dic::Protocol::ImportActionRequest>();
    requestPtr.get()->params.path.emplace_back("jjjj");
    importActionHandler.HandleRequest(std::move(requestPtr));
}