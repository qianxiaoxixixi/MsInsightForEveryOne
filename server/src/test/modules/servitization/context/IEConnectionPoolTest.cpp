/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include <gtest/gtest.h>
#include "IEConnectionPool.h"

using namespace Dic::Module::IE;

class IEConnectionPoolTest : public testing::Test {
};

TEST_F(IEConnectionPoolTest, AllFunc)
{
    std::recursive_mutex mutex;
    IEConnectionPool pool("test", mutex);
    EXPECT_EQ(pool.GetDbPath(), "test");
    EXPECT_TRUE(pool.GetConnection() != nullptr);
    EXPECT_NO_THROW(pool.SetMaxActiveCount());
    EXPECT_NO_THROW(pool.Stop());
}