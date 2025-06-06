/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include "CommunicationBaseAnalyzer.h"
#include "RetransmissionAnalyzer.h"
#include "BandwidthContentionAnalyzer.h"
namespace Dic {
namespace Module {
namespace Communication {
class CommunicationBaseAnalyzerTest : public CommunicationBaseAnalyzer, public testing::Test {
public:
    CommunicationBaseAnalyzerTest() = default;
    ~CommunicationBaseAnalyzerTest() override = default;
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

TEST_F(CommunicationBaseAnalyzerTest, GenerateAdvisorTest)
{
    Dic::Protocol::CommunicationAdvisorInfo info;
    bool result = GenerateAdvisor(info, "compare");
    ASSERT_TRUE(result);
}

TEST_F(CommunicationBaseAnalyzerTest, AllTestAnalyzer)
{
    RetransmissionAnalyzer analyzer1;
    CommunicationAdvisorInfo info;
    analyzer1.GenerateAdvisor(info, "test");
    analyzer1.ComputeStatistics();
    BandwidthContentionAnalyzer analyzer2;
    analyzer2.GenerateAdvisor(info, "test");
}

TEST_F(CommunicationBaseAnalyzerTest, RetransmissionAnalyzer)
{
    RetransmissionAnalyzer analyzer;
    Dic::Protocol::CommunicationAdvisorInfo info;
    EXPECT_NO_THROW(analyzer.QueryAdvisorData(""));
    EXPECT_NO_THROW(analyzer.AssembleAdvisor(info));
}
}
}
}
