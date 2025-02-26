/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include <memory>
#include "ParallelStrategyAlgorithmManager.h"
using namespace Dic::Module;
using namespace Dic::Module::Summary;
class ParallelStrategyAlgorithmManagerTest : public ::testing::Test {
public:
    void SetUp() override
    {
        ParallelStrategyAlgorithmManager::Instance().Reset();
    }

    void TearDown() override
    {
        ParallelStrategyAlgorithmManager::Instance().Reset();
    }
};

class MockParallelStrategyAlgorithm : public BaseParallelStrategyAlgorithm {
public:
    bool UpdateParallelDimension(const std::string &dimension,
                                 const ParallelStrategyConfig &tmpConfig, std::string &err) override { return true; }
    bool GenerateArrangementByDimension(std::string &err) override { return true; }
    bool GetPerformanceIndicatorByDimension(const GetPerformanceIndicatorParam &params,
                                            const std::unordered_map<std::uint32_t, StepStatistic> &statistic,
                                            std::vector<IndicatorDataStruct> &indicatorData, std::string &err) override
    {
        return true;
    }
    void CalAdviceInfo(const std::string &dimension, std::vector<std::string> &advices,
                       std::vector<IndicatorDataStruct> &indicatorData) override {}
    std::vector<Connection> GetAllCommunicationGroups(std::string &err) override { return {}; }

private:
    ArrangementAndConnectionData data;
};

TEST_F(ParallelStrategyAlgorithmManagerTest, AddAlgorithm_ShouldReturnTrue_WhenProjectAlreadyExists)
{
    std::string projectName = "testProject";
    ParallelStrategyConfig config;
    std::string errMsg;
    bool res = ParallelStrategyAlgorithmManager::Instance().AddOrUpdateAlgorithm(projectName, config, errMsg);
    EXPECT_TRUE(res);
    res = ParallelStrategyAlgorithmManager::Instance().AddOrUpdateAlgorithm(projectName, config, errMsg);
    EXPECT_TRUE(res);
}

TEST_F(ParallelStrategyAlgorithmManagerTest, DeleteAlgorithm_ShouldReturnFalse_DeleteAlgWhenProjectNotExist)
{
    std::string projectName = "NonExistentProject";
    ParallelStrategyConfig config;
    bool result = ParallelStrategyAlgorithmManager::Instance().DeleteAlgorithm(projectName);
    EXPECT_FALSE(result);
}

TEST_F(ParallelStrategyAlgorithmManagerTest, DeleteAlgorithm_ShouldReturnTrue_DeleteAlgWhenProjectExist)
{
    std::string projectName = "ExistentProject";
    ParallelStrategyConfig config;
    auto algPtr = std::make_shared<MockParallelStrategyAlgorithm>();
    std::string errMsg;
    ParallelStrategyAlgorithmManager::Instance().AddOrUpdateAlgorithm(projectName, config, errMsg);
    bool result = ParallelStrategyAlgorithmManager::Instance().DeleteAlgorithm(projectName);
    EXPECT_TRUE(result);
}

TEST_F(ParallelStrategyAlgorithmManagerTest, GetAlgorithmByProjectName_ShouldReturnNull_WhenProjectNameNotExist)
{
    std::string err;
    std::shared_ptr<BaseParallelStrategyAlgorithm> result =
            ParallelStrategyAlgorithmManager::Instance().GetAlgorithmByProjectName("NonExistentProject");
    EXPECT_EQ(result, nullptr);
}

TEST_F(ParallelStrategyAlgorithmManagerTest, GetAlgorithmByProjectName_ShouldReturnAlgorithm_WhenProjectNameExist)
{
    std::string err;
    std::shared_ptr<BaseParallelStrategyAlgorithm> algPtr = std::make_shared<MockParallelStrategyAlgorithm>();
    std::string projectName = "ExistentProject";
    ParallelStrategyConfig config;
    std::string errMsg;
    ParallelStrategyAlgorithmManager::Instance().AddOrUpdateAlgorithm(projectName, config, errMsg);
    std::shared_ptr<BaseParallelStrategyAlgorithm> result =
        ParallelStrategyAlgorithmManager::Instance().GetAlgorithmByProjectName("ExistentProject");
    EXPECT_NE(result, nullptr);
}
