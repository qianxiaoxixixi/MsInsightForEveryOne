/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "SummaryProtocolRequest.h"
#include "SummaryProtocolResponse.h"
#include "../../timeline/handler/HandlerTest.cpp"
#include "QueryFwdBwdTimelineHandler.h"

using namespace Dic::Module;
using namespace Dic::Module::Summary;
TEST_F(HandlerTest, QueryFwdBwdTimelineHandlerHandleRequestReturnFalseWhenWrongParameter)
{
    auto request = std::make_unique<PipelineFwdBwdTimelineRequest>();
    request->params.stageId = "";
    request->params.stepId = "";
    QueryFwdBwdTimelineHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, QueryFwdBwdTimelineHandlerHandleRequestReturnTrueWhenNormalParameter)
{
    auto request = std::make_unique<PipelineFwdBwdTimelineRequest>();
    request->params.stageId = "(0)";
    request->params.stepId = "1";
    QueryFwdBwdTimelineHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, true);
}