/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "../../timeline/handler/HandlerTest.cpp"
#include "GlobalProtocolRequest.h"
#include "GetProjectExplorerInfoHandler.h"

TEST_F(HandlerTest, GetProjectExplorerInfoWithEmptyResult)
{
    auto request = std::make_unique<Dic::Module::ProjectExplorerInfoGetRequest>();
    Dic::Module::GetProjectExplorerInfoHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, true);
}