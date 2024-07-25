//
// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
//
#include <gtest/gtest.h>
#include "KernelParse.h"
#include "../../TestSuit.cpp"

class KernelParseTest : TestSuit {
};

TEST_F(TestSuit, KernelParserFileIsEmpty)
{
    std::string currPath = Dic::FileUtil::GetCurrPath();

    Module::Summary::KernelParse &parser = Dic::Module::Summary::KernelParse::Instance();
    EXPECT_EQ(false, parser.Parse(std::vector<std::string>()));
}