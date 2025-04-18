/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <SetCardAliasHandler.h>
#include <gtest/gtest.h>
#include "SearchSliceHandler.h"
#include "HandlerTest.cpp"

class SetCardAliasHandlerTest : HandlerTest {
};

TEST_F(HandlerTest, SetCardAliasHandlerTestNotVariable)
{
    Dic::Module::Timeline::SetCardAliasHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::SetCardAliasRequest>();
    EXPECT_EQ(handler.HandleRequest(std::move(requestPtr)), false);
}

TEST_F(HandlerTest, SetCardAliasHandlerTestInvalidParam)
{
    Dic::Module::Timeline::SetCardAliasHandler handler;
    std::unique_ptr<Dic::Protocol::SetCardAliasRequest> requestPtr =
        std::make_unique<Dic::Protocol::SetCardAliasRequest>();
    requestPtr->command = "unit/setCardAlias";
    requestPtr->projectName = "A";
    requestPtr->params.cardAlias = "alias";
    requestPtr->params.rankId = "$./|";
    EXPECT_EQ(handler.HandleRequest(std::move(requestPtr)), false);
}