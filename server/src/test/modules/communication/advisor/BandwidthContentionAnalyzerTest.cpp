/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include "BandwidthContentionAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {
class BandwidthContentionAnalyzerTest : public BandwidthContentionAnalyzer, public testing::Test {
public:
public:
    BandwidthContentionAnalyzerTest() = default;
    ~BandwidthContentionAnalyzerTest() override = default;
    bool QueryAdvisorData(const std::string &clusterPath) override
    {
        return true;
    }
    void ComputeStatistics() override
    {
        return;
    }
    void AssembleAdvisor(CommunicationAdvisorInfo &info) override
    {
        return;
    }
};

TEST_F(BandwidthContentionAnalyzerTest, QueryAdvisorDataTest)
{
    bool result = QueryAdvisorData("compare");
    ASSERT_TRUE(result);
}

}
}
}
