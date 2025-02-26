/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "ClusterDef.h"
class ClusterDefTest : public ::testing::Test {};

/**
 * 测试ParallelStrategyConfig
 */
using namespace Dic::Module;
TEST_F(ClusterDefTest, TestParallelStrategyConfig)
{
    const int64_t maxSize = Dic::Module::MAX_PARALLEL_SIZE + 1;
    const int64_t minSize = 1;
    ParallelStrategyConfig config1 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, maxSize, minSize, minSize, minSize, minSize };
    ParallelStrategyConfig config2 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, maxSize, minSize, minSize, minSize };
    ParallelStrategyConfig config3 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, minSize, maxSize, minSize, minSize };
    ParallelStrategyConfig config4 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, minSize, minSize, maxSize, minSize };
    ParallelStrategyConfig config5 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, minSize, minSize, minSize, maxSize };
    ParallelStrategyConfig config6 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, 0, minSize, minSize, minSize, minSize };
    ParallelStrategyConfig config7 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, 0, minSize, minSize, minSize };
    ParallelStrategyConfig config8 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, minSize, 0, minSize, minSize };
    ParallelStrategyConfig config9 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, minSize, minSize, 0, minSize };
    ParallelStrategyConfig config10 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, minSize, minSize, minSize, minSize, 0 };
    ParallelStrategyConfig config11 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, 5, 5, 100, 100, 254 };
    ParallelStrategyConfig config12 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, 10000, 10000, 10000, 10000, 100 };
    ParallelStrategyConfig config13 = { MEGATRON_LM_TP_CP_EP_DP_PP_ALG, 2, 2, 4, 2, 2 };
    ParallelStrategyConfig config14 = { MINDSPEED_TP_CP_EP_DP_PP_ALG, 5, 5, 10, 10, 254};
    std::string error;
    EXPECT_EQ(config1.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] PP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config2.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] TP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config3.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] DP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config4.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] CP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config5.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] EP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config6.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] PP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config7.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] TP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config8.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] DP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config9.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] CP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config10.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] EP size must be between 1 and "+ std::to_string(MAX_PARALLEL_SIZE));
    EXPECT_EQ(config11.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] DP size must be evenly divided by EP Size.");
    EXPECT_EQ(config12.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] The product of PP size, TP size, DP size, and CP size must be less than " +
        std::to_string(UINT32_MAX));
    EXPECT_EQ(config14.CheckParams(error), false);
    EXPECT_EQ(error, "[Summary] The product of DP size and CP size must be evenly divided by EP Size.");
    error = "";
    EXPECT_EQ(config13.CheckParams(error), true);
    EXPECT_EQ(error, "");
}
