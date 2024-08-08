/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "DataEngine.h"
#include "RepositoryFactory.h"
#include "DominQuery.h"

using namespace Dic::Module::Timeline;
class DataEngineTest : public ::testing::Test {};

TEST_F(DataEngineTest, QueryAllThreadInfoNormalTest)
{
    std::unordered_map<uint64_t, std::pair<std::string, std::string>> threadInfo;
    EXPECT_NO_THROW({
        Dic::Module::Timeline::DataEngine dataEngine;
        std::shared_ptr<Dic::Module::Timeline::RepositoryFactoryInterface> repositoryFactoryInterface =
            std::make_shared<Dic::Module::Timeline::RepositoryFactory>();
        dataEngine.SetRepositoryFactory(repositoryFactoryInterface);
        ThreadQuery threadQuery;
        threadQuery.metaType = PROCESS_TYPE::CANN_API;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
        threadQuery.metaType = PROCESS_TYPE::MS_TX;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
        threadQuery.metaType = PROCESS_TYPE::API;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
        threadQuery.metaType = PROCESS_TYPE::OVERLAP_ANALYSIS;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
        threadQuery.metaType = PROCESS_TYPE::ASCEND_HARDWARE;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
        threadQuery.metaType = PROCESS_TYPE::HCCL;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
        threadQuery.metaType = PROCESS_TYPE::NONE;
        dataEngine.QueryAllThreadInfo(threadQuery, threadInfo);
    });
}