/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include <gtest/gtest.h>
#include "ProjectParserIpynb.h"

using namespace Dic::Module;

class ParserIpynbTest : public testing::Test {
};

TEST_F(ParserIpynbTest, SendResponse)
{
    ProjectParserIpynb parser;
    ProjectExplorerInfo project;
    std::string msg;
    Dic::Module::ImportActionRequest request;
    EXPECT_NO_THROW(parser.IpynbImportResponse(request, project, false));
}

TEST_F(ParserIpynbTest, SendEvent)
{
    EXPECT_NO_THROW(ProjectParserIpynb::SendJupyterInfo("test"));
}

TEST_F(ParserIpynbTest, JupyterProcess)
{
    EXPECT_NO_THROW(ProjectParserIpynb::JupyterProcess("test"));
}

TEST_F(ParserIpynbTest, GetImportPath)
{
    std::string err;
    ProjectParserIpynb parser;
    EXPECT_EQ(parser.GetParseFileByImportFile("test", err).size(), 1);
}
