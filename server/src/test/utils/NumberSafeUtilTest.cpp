/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "../TestSuit.cpp"
#include "NumberSafeUtil.h"

using namespace Dic;
using namespace NumberSafe;
TEST(NumberSafe, SafeCast)
{
    // int -> uint
    int32_t a = -10;
    bool suc = IsSafeCast<int32_t, uint32_t>(a);
    EXPECT_EQ(suc, false);
    suc = IsSafeCast<int, uint32_t>(100);
    EXPECT_TRUE(suc);
    suc = IsSafeCast<int, uint32_t>(std::numeric_limits<int>::max());
    EXPECT_TRUE(suc);
    suc = IsSafeCast<uint32_t, int>(std::numeric_limits<int>::min());
    EXPECT_FALSE(suc);
    // int -> double
    suc = IsSafeCast<int, double>(a);
    EXPECT_EQ(suc, true);
    suc = IsSafeCast<int, double>(std::numeric_limits<int>::min());
    EXPECT_TRUE(suc);
    // uint -> int
    suc = IsSafeCast<uint32_t, int32_t>(100);
    EXPECT_TRUE(suc);
    suc = IsSafeCast<uint32_t, int32_t>(0);
    EXPECT_TRUE(suc);
    uint32_t b = std::numeric_limits<uint32_t>::max();
    suc = IsSafeCast<uint32_t, int32_t>(b);
    EXPECT_FALSE(suc);
    uint64_t c = std::numeric_limits<uint64_t>::max();
    suc = IsSafeCast<uint64_t, uint32_t>(c);
    EXPECT_FALSE(suc);
    suc = IsSafeCast<uint64_t, int>(-10);
    EXPECT_FALSE(suc);
    // double -> int
    suc = IsSafeCast<double, uint32_t>(10.11);
    EXPECT_TRUE(suc);
}

TEST(NumberSafe, FLIP)
{
    int a = 10;
    a = Flip(a);
    EXPECT_EQ(a, -10);
    a = 0;
    EXPECT_EQ(0, Flip(a));
    int64_t b = 100;
    EXPECT_EQ(-100, Flip(b));
}

TEST(NumberSafe, AddTowInt)
{
    // int + int
    int a = 10;
    int b = 11;
    int res = Add(a, b);
    EXPECT_EQ(res, 21);
    a = -10;
    b = 5;
    res = Add(a, b);
    EXPECT_EQ(res, -5);
    a = std::numeric_limits<int>::max();
    b = 10;
    res = Add(a, b);
    EXPECT_EQ(res, 0);
    a = std::numeric_limits<int>::min();
    res = Add(a, b);
    EXPECT_EQ(res, std::numeric_limits<int>::min() + 10);
}

TEST(NumerSafe, AddIntWithUint)
{
    int32_t a = 10;
    uint32_t b = 100;
    auto res = Add(a, b);
    EXPECT_EQ(res, 110);
    a = -10;
    EXPECT_EQ(Add(a, b), 0);
    a = 10;
    b = std::numeric_limits<uint32_t>::max();
    EXPECT_EQ(Add(a, b), 0);
}

TEST(NumberSafe, AddInt64WithInt32)
{
    int32_t a = 10;
    int64_t b = 20;
    EXPECT_EQ(Add(a, b), 30);
    b = static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 1;
    EXPECT_EQ(Add(a, b), static_cast<int64_t>(std::numeric_limits<int32_t>::max()) + 11);
    b = std::numeric_limits<int64_t>::max();
    EXPECT_EQ(Add(a, b), 0);
}

TEST(NumberSafe, AddFloatPoint)
{
    double a = 1.71;
    double b = 2.90;
    EXPECT_EQ(Add(a, b), 1.71 + 2.90);
}


TEST(NumberSafe, Sub)
{
    // int to int
    int32_t a = 10;
    int32_t b = 100;
    EXPECT_EQ(Sub(a, b), -90);
    b = 2;
    EXPECT_EQ(Sub(a, b), 8);
    b = -10;
    EXPECT_EQ(Sub(a, b), 20);
}

TEST(NumberSafe, Multiply)
{
    int32_t a = 10;
    int32_t b = 20;
    EXPECT_EQ(Muls(a, b), 200);
    b = 0;
    EXPECT_EQ(Muls(a, b), 0);
    a = 1 << 2;
    b = 1 << (sizeof(int32_t) * 8 - 1);
    EXPECT_EQ(Muls(a, b), 0);
}

TEST(NumberSafe, Div)
{
    uint64_t a = 10;
    uint64_t b = 0;
    EXPECT_EQ(Division(a, b), 0);
}
