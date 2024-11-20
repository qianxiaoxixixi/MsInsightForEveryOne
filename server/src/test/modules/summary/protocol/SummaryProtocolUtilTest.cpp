/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include <GlobalDefs.h>
#include "ProtocolDefs.h"
#include "SummaryProtocol.h"
#include "SummaryProtocolUtil.h"
#include "SummaryProtocolRequest.h"
#include "SummaryProtocolResponse.h"

using namespace Dic::Protocol;
class SummaryProtocolUtilTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        protocol.Register();
    }

    void TearDown() override
    {
        protocol.UnRegister();
    }

    Dic::Protocol::SummaryProtocol protocol;
};

TEST_F(SummaryProtocolUtilTest, ToQueryParallelStrategyRequestTest)
{
    std::string reqJson = R"({"id": 2, "moduleName": "summary", "type": "request",
        "command": "summary/query/parallelStrategy", "resultCallbackId": 0, "params": {}})";
    Dic::document_t json;
    json.Parse(reqJson.c_str());
    std::string err;
    auto result = dynamic_cast<QueryParallelStrategyRequest &>(*(protocol.FromJson(json, err)));
    EXPECT_EQ(result.command, REQ_RES_SUMMARY_QUERY_PARALLEL_STRATEGY);
}


TEST_F(SummaryProtocolUtilTest, ToSetParallelStrategyRequestTest)
{
    std::string reqJson = R"({"id": 2, "moduleName": "summary", "type": "request",
        "command": "summary/set/parallelStrategy", "resultCallbackId": 0, "params": {}})";
    Dic::document_t json;
    json.Parse(reqJson.c_str());
    std::string err;
    auto request = protocol.FromJson(json, err);
    EXPECT_TRUE(request == nullptr);
    reqJson = R"({"id": 2, "moduleName": "summary", "type": "request",
        "command": "summary/set/parallelStrategy", "resultCallbackId": 0, "params":
        {"algorithm": "test", "tpSize": 2, "ppSize": 3, "dpSize": 4}})";
    json.Parse(reqJson.c_str());
    auto result = dynamic_cast<SetParallelStrategyRequest &>(*(protocol.FromJson(json, err)));
    EXPECT_EQ(result.command, REQ_RES_SUMMARY_SET_PARALLEL_STRATEGY);
    EXPECT_EQ(result.config.algorithm, "test");
    EXPECT_EQ(result.config.tpSize, 2); // tp = 2
    EXPECT_EQ(result.config.ppSize, 3); // pp = 3
    EXPECT_EQ(result.config.dpSize, 4); // dp = 4
}

TEST_F(SummaryProtocolUtilTest, ToQueryParallelStrategyResponseTest)
{
    Dic::Protocol::QueryParallelStrategyResponse response;
    std::string err;
    response.config.algorithm = "megatron-lm";
    response.config.tpSize = 8; // tp = 8
    response.config.ppSize = 4; // pp = 4
    response.config.dpSize = 2; // dp = 2
    std::optional<Dic::document_t> jsonOptional = protocol.ToJson(response, err);
    EXPECT_EQ(jsonOptional.value()["body"][KEY_ALGORITHM.c_str()], response.config.algorithm.c_str());
    EXPECT_EQ(jsonOptional.value()["body"][KEY_TP_SIZE.c_str()], response.config.tpSize);
    EXPECT_EQ(jsonOptional.value()["body"][KEY_PP_SIZE.c_str()], response.config.ppSize);
    EXPECT_EQ(jsonOptional.value()["body"][KEY_DP_SIZE.c_str()], response.config.dpSize);
}

TEST_F(SummaryProtocolUtilTest, ToSetParallelStrategyResponseTest)
{
    Dic::Protocol::SetParallelStrategyResponse response;
    std::string err;
    response.result = false;
    response.msg = "test";
    std::optional<Dic::document_t> jsonOptional = protocol.ToJson(response, err);
    EXPECT_EQ(jsonOptional.value()["body"][KEY_RESULT.c_str()], response.result);
    EXPECT_EQ(jsonOptional.value()["body"][KEY_MSG.c_str()], response.msg.c_str());
}

TEST_F(SummaryProtocolUtilTest, ToQueryFwdBwdTimelineRequestWillReturnNullWhenInputWrong)
{
    std::string reqJson = R"({"id": 2, "moduleName": "summary", "type": "request",
        "command": "parallelism/pipeline/fwdBwdTimeline", "resultCallbackId": 0, "params": {}})";
    Dic::document_t json;
    json.Parse(reqJson.c_str());
    std::string err;
    auto request = protocol.FromJson(json, err);
    EXPECT_TRUE(request == nullptr);
    reqJson = R"({"id": 2, "moduleName": "summary", "type": "request",
        "command": "parallelism/pipeline/fwdBwdTimeline", "resultCallbackId": 0, "params": {"step": "2"}})";
    json.Parse(reqJson.c_str());
    request = protocol.FromJson(json, err);
    EXPECT_TRUE(request == nullptr);
}

TEST_F(SummaryProtocolUtilTest, ToQueryFwdBwdTimelineRequestWillReturnTrueWhenInputCorrect)
{
    std::string reqJson = R"({"id": 2, "moduleName": "summary", "type": "request", "resultCallbackId": 0,
        "command": "parallelism/pipeline/fwdBwdTimeline", "params": {"stepId": "2", "stageId": "3"}})";
    Dic::document_t json;
    json.Parse(reqJson.c_str());
    std::string err;
    auto result = dynamic_cast<PipelineFwdBwdTimelineRequest &>(*(protocol.FromJson(json, err)));
    EXPECT_EQ(result.command, REQ_RES_PIPELINE_FWD_BWD_TIMELINE);
    EXPECT_EQ(result.params.stepId, "2");
    EXPECT_EQ(result.params.stageId, "3");
}

TEST_F(SummaryProtocolUtilTest, ToQueryFwdBwdTimelineResponseTestWillReturnWhenEmptyInput)
{
    Dic::Protocol::PipelineFwdBwdTimelineResponse response{};
    std::string err;
    std::optional<Dic::document_t> jsonOptional = protocol.ToJson(response, err);
    EXPECT_EQ(jsonOptional.has_value(), true);
    EXPECT_EQ(jsonOptional.value().HasMember("body"), true);
    EXPECT_EQ(jsonOptional.value()["body"].HasMember("minTime"), true);
    EXPECT_EQ(jsonOptional.value()["body"].HasMember("minTime"), true);
    EXPECT_EQ(jsonOptional.value()["body"].HasMember("rankList"), true);
    EXPECT_EQ(jsonOptional.value()["body"]["rankList"].GetArray().Size(), 0);
}

TEST_F(SummaryProtocolUtilTest, ToQueryFwdBwdTimelineResponseTestWillReturnWhenNormalInput)
{
    Dic::Protocol::PipelineFwdBwdTimelineResponse response{};
    response.body.maxTime = 10086; // 10086
    response.body.minTime = 10010; // 10010
    response.body.rankLists = {"0", "1"};
    PipelineFwdBwdTimelineByComponent rank0FwdBwd = {
        "FWD/BWD", {
            {"FP", 3000, 123456, 135678, 0, "FWD/BWD", "0", "1", "FWD"},
            {"BP", 5000, 147890, 234567, 0, "FWD/BWD", "0", "1", "BWD"}
        }
    };
    PipelineFwdBwdTimelineByComponent rank0P2P = {
        "P2P", {{"hcom_send", 3000, 136789, 137890, 0, "P2P", "0", "1", "SEND"}}
    };
    PipelineFwdBwdTimelineByRank rank0 = {"0", {"FWD/BWD", "P2P"}, {rank0FwdBwd, rank0P2P}};
    PipelineFwdBwdTimelineByComponent rank1FwdBwd = {
        "FWD/BWD", {
            {"FP", 3000, 123456, 135678, 0, "FWD/BWD", "0", "1", "FWD"}
        }
    };
    PipelineFwdBwdTimelineByRank rank1 = {"1", {"FWD/BWD"}, {rank1FwdBwd}};

    response.body.rankDataList = {rank0, rank1};
    std::string err;
    std::optional<Dic::document_t> jsonOptional = protocol.ToJson(response, err);
    EXPECT_EQ(jsonOptional.has_value(), true);
    EXPECT_EQ(jsonOptional.value().HasMember("body"), true);
    EXPECT_EQ(jsonOptional.value()["body"].HasMember("minTime"), true);
    EXPECT_EQ(jsonOptional.value()["body"]["minTime"], response.body.minTime);
    EXPECT_EQ(jsonOptional.value()["body"].HasMember("rankList"), true);
    EXPECT_EQ(jsonOptional.value()["body"]["rankList"].GetArray().Size(), response.body.rankDataList.size());
    int i = 0;
    for (const auto &item : jsonOptional.value()["body"]["rankList"].GetArray()) {
        auto tmp = response.body.rankDataList.at(i);
        EXPECT_EQ(item.HasMember("rank"), true);
        EXPECT_EQ(item["rank"].GetString(), tmp.rankId);
        EXPECT_EQ(item.HasMember("componentList"), true);
        EXPECT_EQ(item["componentList"].GetArray().Size(), tmp.componentDataList.size());
        int j = 0;
        for (const auto &componentItem : item["componentList"].GetArray()) {
            auto componentTmp = tmp.componentDataList.at(j);
            EXPECT_EQ(componentItem.HasMember("component"), true);
            EXPECT_EQ(componentItem["component"].GetString(), componentTmp.component);
            EXPECT_EQ(componentItem.HasMember("traceList"), true);
            EXPECT_EQ(componentItem["traceList"].GetArray().Size(), componentTmp.traceList.size());
            j++;
        }
        i++;
    }
}