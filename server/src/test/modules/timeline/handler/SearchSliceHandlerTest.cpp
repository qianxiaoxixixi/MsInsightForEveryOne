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
    EXPECT_EQ(handler.HandleRequest(std::move(requestPtr)), false);
}

TEST_F(HandlerTest, SearchSliceHandlerTestInvalidParam)
{
    Dic::Module::Timeline::SearchSliceHandler handler;
    std::unique_ptr<Dic::Protocol::SearchSliceRequest> requestPtr =
        std::make_unique<Dic::Protocol::SearchSliceRequest>();
    requestPtr->params.metadataList.emplace_back(Dic::Protocol::Metadata {
        .lockStartTime = 2, .lockEndTime = 1
    });
    EXPECT_EQ(handler.HandleRequest(std::move(requestPtr)), false);
}