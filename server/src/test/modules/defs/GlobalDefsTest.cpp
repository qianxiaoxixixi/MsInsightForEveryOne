/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "GlobalDefs.h"

using namespace Dic;
class GlobalDefsTest : public ::testing::Test {
};

TEST_F(GlobalDefsTest, isFileConflict)
{
    ProjectTypeEnum typeEnumIPYNB = ProjectTypeEnum::IPYNB;
    ProjectTypeEnum typeEnumDB = ProjectTypeEnum::DB;
    EXPECT_TRUE(isFileConflict(typeEnumIPYNB, typeEnumDB));
    EXPECT_FALSE(isFileConflict(typeEnumDB, typeEnumDB));
}