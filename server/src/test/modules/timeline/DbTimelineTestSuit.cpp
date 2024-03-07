/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include "../../FullDbTestSuit.cpp"

class DbTimelineTestSuit : FullDbTestSuit {
};

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceNameCount)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    int expectCount = 0;
    Dic::Protocol::SearchCountParams params;
    params.searchContent = "hcom";
    params.rankId = 2; // cardId = 2

    auto count = database->SearchSliceNameCount(params);
    EXPECT_EQ(count, expectCount);
}

TEST_F(FullDbTestSuit, FullDb_of_SearchSliceName)
{
    auto database = Dic::Module::Timeline::DataBaseManager::Instance().GetTraceDatabase("FullDb");
    std::string sliceName = "hcom";
    int index = 0;
    uint64_t minTimestamp = 0;
    Dic::Protocol::SearchSliceBody body;

    body.rankId = 2; // cardId = 2
    std::string expectPid = "";
    std::string expectTid = "";
    uint64_t expectStartTime = 0;
    int32_t expectDepth = 0;
    uint64_t expectDuration = 0;

    database->SearchSliceName(sliceName, index, minTimestamp, body);
    EXPECT_EQ(body.pid, expectPid);
    EXPECT_EQ(body.tid, expectTid);
    EXPECT_EQ(body.startTime, expectStartTime);
    EXPECT_EQ(body.depth, expectDepth);
    EXPECT_EQ(body.duration, expectDuration);
}
