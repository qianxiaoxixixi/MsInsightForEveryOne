/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "../TestSuit.cpp"
#include "NumberUtil.h"

using namespace Dic;

TEST(NumberUtil, TryParseInt) {
    EXPECT_EQ(NumberUtil::TryParseInt("1"), 1);
    EXPECT_EQ(NumberUtil::TryParseInt("129"), 129);
    EXPECT_EQ(NumberUtil::TryParseInt("1024"), 1024);
    EXPECT_EQ(NumberUtil::TryParseInt("A"), INVALID_NUMBER);
    EXPECT_EQ(NumberUtil::TryParseInt("99999999999999999999"), INVALID_NUMBER);
}

TEST(NumberUtil, Uint64ToHexString) {
    EXPECT_EQ(NumberUtil::Uint64ToHexString(1), "0x1");
    EXPECT_EQ(NumberUtil::Uint64ToHexString(2), "0x2");
    EXPECT_EQ(NumberUtil::Uint64ToHexString(10), "0xa");
    EXPECT_EQ(NumberUtil::Uint64ToHexString(11), "0xb");
    EXPECT_EQ(NumberUtil::Uint64ToHexString(999999999), "0x3b9ac9ff");
    EXPECT_EQ(NumberUtil::Uint64ToHexString(99999999999999), "0x5af3107a3fff");
}

TEST(NumberUtil, HexadecimalStrToDecimalInt) {
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0x1"), 1);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0x2"), 2);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0xa"), 10);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0xb"), 11);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0x3b9ac9ff"), 999999999);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0xyyyy2"), INVALID_NUMBER);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("xxxxxxx"), INVALID_NUMBER);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0x5af35af35af35af35af35af35af35af35af35af35af3"), INVALID_NUMBER);
    EXPECT_EQ(NumberUtil::HexadecimalStrToDecimalInt("0x3b9ac9ff3b9ac9ff3b9ac9ff3b9ac9ff3b9ac9ff3b9ac9ff"),
              INVALID_NUMBER);
}