/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "ClusterFileParser.h"
#include "DbClusterDataBase.h"
#include "../../TestSuit.cpp"

class ClusterParseTest : public ::testing::Test {
};

TEST_F(ClusterParseTest, ParserDbClusterFailWithEmptyPath)
{
    std::recursive_mutex mutex;
    std::string clusterDbPath;
    auto database = std::make_shared<Dic::Module::FullDb::DbClusterDataBase>(mutex);
    ClusterFileParser clusterFileParser(clusterDbPath, database, "Test");
    bool res = clusterFileParser.ParserClusterOfDb();
    EXPECT_EQ(res, false);
}