/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include <gtest/gtest.h>
#include "MindIELLMParallelStrategyAlgorithm.h"
using namespace Dic::Module;
using namespace Dic::Protocol;
using namespace Dic::Module::Summary;
class MindIELLMParallelStrategyAlgorithmTest : public ::testing::Test {
};

TEST_F(MindIELLMParallelStrategyAlgorithmTest, UpdateParallelDimension_ShouldReturnTrue_WhenUpdateSuccess)
{
    MindIELLMParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_TP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 4; // 4
    config.epSize = 4; // 4
    config.moeTpSize = 2; // 2
    config.algorithm = MINDIE_LLM_TP_DP_EP_PP_MOETP_ALG;
    std::string err;
    bool res = algorithm.UpdateParallelDimension(dimension, config, err);
    EXPECT_TRUE(res);
}

TEST_F(MindIELLMParallelStrategyAlgorithmTest, UpdateParallelDimension_ShouldReturnFalse_WhenWrongInput)
{
    MindIELLMParallelStrategyAlgorithm algorithm;
    std::string dimension = "yyyyy";
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 4; // 4
    config.epSize = 4; // 4
    config.moeTpSize = 2; // 2
    config.algorithm = "xxxx";
    std::string err;
    bool res = algorithm.UpdateParallelDimension(dimension, config, err);
    EXPECT_FALSE(res);
    EXPECT_EQ(err, "Failed to update parallel view. Unexpected algorithm for the MOE algorithm.");
    MindIELLMParallelStrategyAlgorithm algorithm2;
    config.algorithm = MINDIE_LLM_TP_DP_EP_PP_MOETP_ALG;
    res = algorithm.UpdateParallelDimension(dimension, config, err);
    EXPECT_FALSE(res);
    EXPECT_EQ(err, "Failed to update show map for parallel view. Unexpected dimension.");
}

TEST_F(MindIELLMParallelStrategyAlgorithmTest, GetArrangementByDimension_ShouldGetArrangement_TestWithTpDimension)
{
    MindIELLMParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_TP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 2; // 2
    config.epSize = 2; // 2
    config.moeTpSize = 2; // 2
    config.algorithm = MINDIE_LLM_TP_DP_EP_PP_MOETP_ALG;
    const std::vector<std::string> EXPECTED_NAME = {
        "dp0-pp0-tp0", "dp0-pp0-tp1", "dp1-pp0-tp0", "dp1-pp0-tp1",
        "dp0-pp1-tp0", "dp0-pp1-tp1", "dp1-pp1-tp0", "dp1-pp1-tp1"};
    const std::vector<Position> EXPECTED_POSITION = {
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, // y = 0
        {0, 1}, {1, 1}, {2, 1}, {3, 1} // y = 1
    };
    const std::vector<std::vector<uint32_t>> EXPECT_RANKS = {
        {0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}
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
    // 20个connections: 'pp': 4, 'dp': 4, 'tp': 4, 'exp': 4, 'moe_tp': 4,
    EXPECT_EQ(data.connections.size(), 20); // 20
}

TEST_F(MindIELLMParallelStrategyAlgorithmTest, GetArrangementByDimension_ShouldGetArrangement_TestWithPpDimension)
{
    MindIELLMParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_PP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 2; // 2
    config.epSize = 2; // 2
    config.moeTpSize = 2; // 2
    config.algorithm = MINDIE_LLM_TP_DP_EP_PP_MOETP_ALG;
    const std::vector<std::string> EXPECTED_NAME = {
        "dp0-pp0", "dp1-pp0", "dp0-pp1", "dp1-pp1"};
    const std::vector<std::vector<uint32_t>> EXPECT_RANKS = {
        {0, 1}, {2, 3}, {4, 5}, {6, 7}
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
        EXPECT_EQ(item.ranks.size(), config.tpSize);
        for (size_t i = 0; i < item.ranks.size(); ++i) {
            EXPECT_EQ(item.ranks.at(i), EXPECT_RANKS[item.index].at(i));
        }
    }
}

TEST_F(MindIELLMParallelStrategyAlgorithmTest, GetArrangementByDimension_ShouldGetArrangement_TestWithDpDimension)
{
    MindIELLMParallelStrategyAlgorithm algorithm;
    std::string dimension = DIMENSIONS_DP;
    ParallelStrategyConfig config;
    config.ppSize = 2; // 2
    config.tpSize = 2; // 2
    config.dpSize = 2; // 2
    config.epSize = 2; // 2
    config.moeTpSize = 2; // 2
    config.algorithm = MINDIE_LLM_TP_DP_EP_PP_MOETP_ALG;
    const std::vector<std::string> EXPECTED_NAME = {
        "dp0", "dp1"};
    const std::vector<std::vector<uint32_t>> EXPECT_RANKS = {
        {0, 1, 4, 5}, {2, 3, 6, 7}
    };
    const std::vector<Position> EXPECTED_POSITION = {{0, 0}, {1, 0}}; // position(x, y)
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