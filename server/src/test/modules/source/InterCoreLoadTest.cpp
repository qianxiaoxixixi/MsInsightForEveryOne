/*
* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/

#include <gtest/gtest.h>
#include "InterCoreLoadGraphParser.h"
#include "InterCoreLoadTest.h"

namespace Dic::Module::Source::Test {
using namespace Dic::Module::Source;
using namespace Dic::Protocol;

class InterCoreLoadTest : public ::testing::Test {};

TEST_F(InterCoreLoadTest, test_GetInterCoreLoadAnalysisInfo_with_normal_json)
{
    InterCoreLoadGraphParser parser;
    DetailsInterCoreLoadGraphBody body;
    parser.GetInterCoreLoadAnalysisInfo(INTER_CORE_LOAD_ANALYSIS_JSON, body);
    // 校验解析的结果
    ASSERT_STREQ(body.soc.c_str(), "Ascend910B4");
    ASSERT_STREQ(body.opType.c_str(), "mix");
    ASSERT_STREQ(body.advice.c_str(), "\t0) vector core0 subcore1 took more time than other core.\n");
    int opDetailSize = 1;
    ASSERT_EQ(body.opDetails.size(), opDetailSize);
    DetailsInterCoreLoadOpDetail &opDetail = body.opDetails[0];
    int coreId = 0;
    ASSERT_EQ(opDetail.coreId, coreId);
    int coreDetailSize = 3;
    ASSERT_EQ(opDetail.subCoreDetails.size(), coreDetailSize);
    DetailsInterCoreLoadSubCoreDetail &subCoreDetail = opDetail.subCoreDetails[0];
    ASSERT_STREQ(subCoreDetail.subCoreName.c_str(), "cube0");
    uint64_t cycles = 135938;
    int cycleLevel = 3;
    ASSERT_EQ(subCoreDetail.cycles.value, cycles);
    ASSERT_EQ(subCoreDetail.cycles.level, cycleLevel);
    float throughput = 46.014137f;
    int throughputLevel = 1;
    ASSERT_FLOAT_EQ(subCoreDetail.throughput.value, throughput);
    ASSERT_EQ(subCoreDetail.throughput.level, throughputLevel);
    float hitRate = 63.994083f;
    int hitRateLevel = 6;
    ASSERT_FLOAT_EQ(subCoreDetail.cacheHitRate.value, hitRate);
    ASSERT_EQ(subCoreDetail.cacheHitRate.level, hitRateLevel);
}
}