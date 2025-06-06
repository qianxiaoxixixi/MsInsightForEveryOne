/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "SafeQueue.h"

using namespace Dic;

class SafeQueueTest : public testing::Test {
};

TEST_F(SafeQueueTest, testPop)
{
    SafeQueue<int> q;
    int res = 1; // push 1
    q.Push(res);
    EXPECT_EQ(q.Empty(), false);
    EXPECT_EQ(q.Size(), 1); // size 1
    int res2 = 0;
    q.Pop(res2);
    EXPECT_EQ(res, 1);
    q.Push(res);
    q.Clear();
    EXPECT_EQ(q.Empty(), true);
}