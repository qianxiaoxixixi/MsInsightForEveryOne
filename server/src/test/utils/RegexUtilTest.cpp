/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "../TestSuit.h"
#include "RegexUtil.h"

using namespace Dic;


TEST(RegexUtil, RegexSearch) {
    std::string data = "AaaAaaaA";
    std::string pattern;
    std::optional<std::smatch> smatch;

    pattern = "a+";
    smatch = RegexUtil::RegexSearch(data, pattern);
    EXPECT_EQ(smatch->str(), "aa");

    pattern = "aaAa+";
    smatch = RegexUtil::RegexSearch(data, pattern);
    EXPECT_EQ(smatch->str(), "aaAaaa");

    pattern = "AaaAaaaA";
    smatch = RegexUtil::RegexSearch(data, pattern);
    EXPECT_EQ(smatch->str(), "AaaAaaaA");

    pattern = "ww";
    smatch = RegexUtil::RegexSearch(data, pattern);
    EXPECT_EQ(smatch.has_value(), false);
}

TEST(RegexUtil, RegexMatch) {
    std::string data = "AaaAaaaA";
    std::string pattern;
    std::optional<std::smatch> smatch;

    pattern = "AaaAaaaA";
    smatch = RegexUtil::RegexMatch(data, pattern);
    EXPECT_EQ(smatch.value()[0], "AaaAaaaA");

    pattern = "Aa+Aa+aA";
    smatch = RegexUtil::RegexMatch(data, pattern);
    EXPECT_EQ(smatch.value()[0], "AaaAaaaA");

    pattern = "\\w+";
    smatch = RegexUtil::RegexMatch(data, pattern);
    EXPECT_EQ(smatch.value()[0], "AaaAaaaA");

    pattern = "[a-zA-z]+";
    smatch = RegexUtil::RegexMatch(data, pattern);
    EXPECT_EQ(smatch.value()[0], "AaaAaaaA");
}