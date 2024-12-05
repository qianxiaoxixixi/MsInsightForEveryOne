/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "SummaryProtocolRequest.h"
#include "../../timeline/handler/HandlerTest.cpp"
#include "QueryFwdBwdTimelineHandler.h"
#include "QueryCommunicationDetailHandler.h"
#include "QueryComputeDetailInfoHandler.h"
#include "QueryParallelStrategyConfigHandler.h"
#include "RankAndBubbleTimeHandler.h"
#include "SetParallelStrategyConfigHandler.h"
#include "StageAndBubbleTimeHandler.h"
#include "StageHandler.h"
#include "StepHandler.h"

using namespace Dic::Module;
using namespace Dic::Module::Summary;

const int NUMBER_FIVE_HUNDRED = 500;
const int NUMBER_TEN = 10;
const int NUMBER_ONE = 1;

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

TEST_F(HandlerTest, QueryCommunicationDetailHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<CommunicationDetailRequest>();
    request->params.rankId = "100";
    request->params.currentPage = NUMBER_ONE;
    request->params.pageSize = NUMBER_TEN;
    request->params.orderBy = "orderBy";
    request->params.order = "order";
    QueryCommunicationDetailHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, QueryComputeDetailInfoHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<ComputeDetailRequest>();
    request->params.rankId = "100";
    request->params.currentPage = NUMBER_ONE;
    request->params.pageSize = NUMBER_TEN;
    request->params.orderBy = "orderBy";
    request->params.order = "order";
    request->params.timeFlag = "flag";
    QueryComputeDetailInfoHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, QueryParallelStrategyConfigHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<QueryParallelStrategyRequest>();
    QueryParallelStrategyConfigHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, RankAndBubbleTimeHandlerWithStepIdError)
{
    auto request = std::make_unique<PipelineRankTimeRequest>();
    request->params.stepId = ";";
    request->params.stageId = "1";
    RankAndBubbleTimeHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, RankAndBubbleTimeHandlerWithStageIdError)
{
    auto request = std::make_unique<PipelineRankTimeRequest>();
    request->params.stepId = "1";
    request->params.stageId = ";";
    RankAndBubbleTimeHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, RankAndBubbleTimeHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<PipelineRankTimeRequest>();
    request->params.stepId = "1";
    request->params.stageId = "1";
    RankAndBubbleTimeHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, SetParallelStrategyConfigHandlerWithParamError)
{
    auto request = std::make_unique<SetParallelStrategyRequest>();
    request->config.tpSize = NUMBER_FIVE_HUNDRED;
    request->config.dpSize = NUMBER_TEN;
    request->config.cpSize = NUMBER_TEN;
    request->config.ppSize = NUMBER_TEN;
    request->config.epSize = NUMBER_TEN;
    SetParallelStrategyConfigHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, SetParallelStrategyConfigHandlerWithExecuteSqlError)
{
    auto request = std::make_unique<SetParallelStrategyRequest>();
    request->config.tpSize = NUMBER_TEN;
    request->config.dpSize = NUMBER_TEN;
    request->config.cpSize = NUMBER_TEN;
    request->config.ppSize = NUMBER_TEN;
    request->config.epSize = NUMBER_TEN;
    SetParallelStrategyConfigHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, StageAndBubbleTimeHandlerWithParamError)
{
    auto request = std::make_unique<PipelineStageTimeRequest>();
    request->params.stepId = ";";
    request->params.stageId = "1";
    StageAndBubbleTimeHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, StageAndBubbleTimeHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<PipelineStageTimeRequest>();
    request->params.stepId = "1";
    request->params.stageId = "1";
    StageAndBubbleTimeHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, StageHandlerWithParamError)
{
    auto request = std::make_unique<PipelineStageRequest>();
    request->params.stepId = ";";
    StageHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, StageHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<PipelineStageRequest>();
    request->params.stepId = "100";
    StageHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}

TEST_F(HandlerTest, StepHandlerWithExecuteSqlFail)
{
    auto request = std::make_unique<PipelineStepRequest>();
    StepHandler handler;
    bool result = handler.HandleRequest(std::move(request));
    EXPECT_EQ(result, false);
}