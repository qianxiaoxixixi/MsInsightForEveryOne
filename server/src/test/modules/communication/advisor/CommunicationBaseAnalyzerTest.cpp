/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include "CommunicationBaseAnalyzer.h"

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
}
}
}
