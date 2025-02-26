/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include <gtest/gtest.h>
#include "MindSpeedParallelStrategyAlgorithm.h"
using namespace Dic::Module;
using namespace Dic::Protocol;
using namespace Dic::Module::Summary;
class MindSpeedParallelStrategyAlgorithmTest : public ::testing::Test {
};

TEST_F(MindSpeedParallelStrategyAlgorithmTest, UpdateParallelDimension_ShouldReturnTrue_WhenUpdateSuccess)
{
    MindSpeedParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_TP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 4; // 4
    config.cpSize = 2; // 2
    config.epSize = 2; // 2
    config.algorithm = MINDSPEED_TP_CP_EP_DP_PP_ALG;
    std::string err;
    bool res = algorithm.UpdateParallelDimension(dimension, config, err);
    EXPECT_TRUE(res);
}
TEST_F(MindSpeedParallelStrategyAlgorithmTest, UpdateParallelDimension_ShouldReturnFalse_WhenWrongInput)
{
    MindSpeedParallelStrategyAlgorithm algorithm;
    std::string dimension = "yyyyy";
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 4; // 4
    config.cpSize = 2; // 2
    config.epSize = 2; // 2
    config.algorithm = "xxxx";
    std::string err;
    bool res = algorithm.UpdateParallelDimension(dimension, config, err);
    EXPECT_FALSE(res);
    EXPECT_EQ(err, "Failed to update parallel view. Unexpected algorithm.");
    MindSpeedParallelStrategyAlgorithm algorithm2;
    config.algorithm = MINDSPEED_TP_CP_EP_DP_PP_ALG;
    res = algorithm.UpdateParallelDimension(dimension, config, err);
    EXPECT_FALSE(res);
    EXPECT_EQ(err, "Failed to update show map for parallel view. Unexpected dimension.");
}

TEST_F(MindSpeedParallelStrategyAlgorithmTest, GetArrangementByDimension_ShouldGetArrangement_TestWithTpDimension)
{
    MindSpeedParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_TP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 2; // 2
    config.cpSize = 2; // 2
    config.epSize = 2; // 2
    config.algorithm = MINDSPEED_TP_CP_EP_DP_PP_ALG;
    const std::vector<std::string> EXPECTED_NAME = {
        "dp0-pp0-cp0-tp0", "dp0-pp0-cp0-tp1", "dp0-pp0-cp1-tp0", "dp0-pp0-cp1-tp1",
        "dp1-pp0-cp0-tp0", "dp1-pp0-cp0-tp1", "dp1-pp0-cp1-tp0", "dp1-pp0-cp1-tp1",
        "dp0-pp1-cp0-tp0", "dp0-pp1-cp0-tp1", "dp0-pp1-cp1-tp0", "dp0-pp1-cp1-tp1",
        "dp1-pp1-cp0-tp0", "dp1-pp1-cp0-tp1", "dp1-pp1-cp1-tp0", "dp1-pp1-cp1-tp1"};
    const std::vector<Position> EXPECTED_POSITION = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, // y = 0
        {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, // y = 1
    };
    const std::vector<std::vector<uint32_t>> EXPECT_RANKS = {
        {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}
    };
    std::string err;
    algorithm.UpdateParallelDimension(dimension, config, err);
    algorithm.GenerateArrangementByDimension(err);
    ArrangementAndConnectionData data = algorithm.GetArrangementData();
    ASSERT_EQ(data.size, EXPECTED_NAME.size());
    for (const auto& item : data.arrangements) {
        EXPECT_EQ(item.name, EXPECTED_NAME[item.index]);
        EXPECT_EQ(item.position, EXPECTED_POSITION[item.index]);
        EXPECT_EQ(item.ranks.size(), 1);
        EXPECT_EQ(item.ranks.at(0), EXPECT_RANKS[item.index].at(0));
    }
}


TEST_F(MindSpeedParallelStrategyAlgorithmTest, GetArrangementByDimension_ShouldGetArrangement_TestWithCpDimension)
{
    MindSpeedParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_CP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 2; // 2
    config.cpSize = 2; // 2
    config.epSize = 2; // 2
    config.algorithm = MINDSPEED_TP_CP_EP_DP_PP_ALG;
    const std::vector<std::string> EXPECTED_NAME = {
        "dp0-pp0-cp0", "dp0-pp0-cp1", "dp1-pp0-cp0", "dp1-pp0-cp1",
        "dp0-pp1-cp0", "dp0-pp1-cp1", "dp1-pp1-cp0", "dp1-pp1-cp1"};
    const std::vector<Position> EXPECTED_POSITION = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}}; // position(x, y)
    const std::vector<std::vector<uint32_t>> EXPECT_RANKS = {
        {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}
    };
    std::string err;
    algorithm.UpdateParallelDimension(dimension, config, err);
    algorithm.GenerateArrangementByDimension(err);
    ArrangementAndConnectionData data = algorithm.GetArrangementData();
    ASSERT_EQ(data.arrangements.size(), EXPECTED_NAME.size());
    for (const auto& item : data.arrangements) {
        EXPECT_EQ(item.name, EXPECTED_NAME[item.index]);
        EXPECT_EQ(item.position, EXPECTED_POSITION[item.index]);
        EXPECT_EQ(item.ranks.size(), config.tpSize);
        for (int64_t i = 0; i < config.tpSize; ++i) {
            EXPECT_EQ(item.ranks.at(i), EXPECT_RANKS[item.index].at(i));
        }
    }
}

TEST_F(MindSpeedParallelStrategyAlgorithmTest, GetArrangementByDimension_ShouldGetArrangement_TestWithPpDimension)
{
    MindSpeedParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_PP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 2; // 2
    config.cpSize = 2; // 2
    config.epSize = 2; // 2
    config.algorithm = MINDSPEED_TP_CP_EP_DP_PP_ALG;
    const std::vector<std::string> EXPECTED_NAME = {
        "dp0-pp0", "dp1-pp0", "dp0-pp1", "dp1-pp1"};
    const std::vector<std::vector<uint32_t>> EXPECT_RANKS = {
        {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}
    };
    const std::vector<Position> EXPECTED_POSITION = {{0, 0}, {1, 0}, {0, 1}, {1, 1}}; // position(x, y)
    std::string err;
    algorithm.UpdateParallelDimension(dimension, config, err);
    algorithm.GenerateArrangementByDimension(err);
    ArrangementAndConnectionData data = algorithm.GetArrangementData();
    ASSERT_EQ(data.arrangements.size(), EXPECTED_NAME.size());
    for (const auto& item : data.arrangements) {
        EXPECT_EQ(item.name, EXPECTED_NAME[item.index]);
        EXPECT_EQ(item.position, EXPECTED_POSITION[item.index]);
        EXPECT_EQ(item.ranks.size(), config.tpSize * config.ppSize);
        for (size_t i = 0; i < item.ranks.size(); ++i) {
            EXPECT_EQ(item.ranks.at(i), EXPECT_RANKS[item.index].at(i));
        }
    }
}