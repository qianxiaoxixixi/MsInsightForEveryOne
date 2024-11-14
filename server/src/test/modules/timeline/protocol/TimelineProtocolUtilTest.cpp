/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "TimelineProtocolUtil.h"
#include "TimelineProtocolResponse.h"
#include "JsonUtil.h"
class TimelineProtocolUtilTest : public ::testing::Test {};

/**
 * 测试ThreadTracesResponseToJson正常情况
 */
TEST_F(TimelineProtocolUtilTest, TestThreadTracesResponseToJsonNormal)
{
    Dic::Protocol::UnitThreadTracesResponse response;
    const uint64_t curDep = 1;
    response.result = true;
    response.body.currentMaxDepth = curDep;
    response.body.havePythonFunction = true;
    const uint64_t maxDep = 2;
    response.body.maxDepth = maxDep;
    Dic::Protocol::ThreadTraces trace;
    trace.name = "kkk";
    const uint64_t end = 4444;
    trace.endTime = end;
    trace.id = "hhh";
    trace.threadId = "lll";
    const uint64_t dur = 777;
    trace.duration = dur;
    const uint64_t str = 444;
    trace.startTime = str;
    trace.pid = "ggg";
    trace.cname = "qqq";
    const uint64_t dep = 1;
    trace.depth = dep;
    std::vector<Dic::Protocol::ThreadTraces> traces;
    traces.emplace_back(trace);
    response.body.data.emplace_back(traces);
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr =
        "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":true,\"command\":\"unit/"
        "threadTraces\",\"moduleName\":\"unknown\",\"body\":{\"maxDepth\":2,\"currentMaxDepth\":1,"
        "\"havePythonFunction\":true,\"data\":[[{\"name\":\"kkk\",\"duration\":777,\"startTime\":444,\"endTime\":4444,"
        "\"depth\":1,\"threadId\":\"lll\",\"cname\":\"qqq\",\"id\":\"hhh\"}]]}}";
    EXPECT_EQ(json, jsonStr);
}

/**
 * 测试ThreadTracesResponseToJson的error情况
 */
TEST_F(TimelineProtocolUtilTest, TestThreadTracesResponseToJsonError)
{
    Dic::Protocol::UnitThreadTracesResponse response;
    response.result = false;
    const uint64_t errorCode = 3;
    Dic::Protocol::ErrorMessage error = { errorCode, "ll" };
    response.error = error;
    auto jsonOp = Dic::Protocol::ToResponseJson(response);
    EXPECT_EQ(jsonOp.has_value(), true);
    const std::string json = Dic::JsonUtil::JsonDump(jsonOp.value());
    const std::string jsonStr = "{\"type\":\"response\",\"id\":0,\"requestId\":0,\"result\":false,\"command\":\"unit/"
        "threadTraces\",\"moduleName\":\"unknown\",\"message\":\"ll\",\"error\":{\"code\":3},\"body\":{\"maxDepth\":0,"
        "\"currentMaxDepth\":0,\"havePythonFunction\":false,\"data\":[]}}";
    EXPECT_EQ(json, jsonStr);
}
