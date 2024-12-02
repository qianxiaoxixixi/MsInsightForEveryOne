/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include "SourceFileParser.h"
#include "SourceProtocolRequest.h"
#include "mockUtils/BinFileGenerator.h"
#include "SourceTest.h"

using namespace std;
using namespace Dic;
using namespace Dic::Module::Source::Test;
class SourceTest : public ::testing::Test {
protected:
    std::string data;
    std::string loadData;
    std::string memoryData;
    std::string baseInfo;
    std::string rooflineDataFile;

    void SetUp() override
    {
        data = "data.bin";
        BinFileGenerator dataFileGenerator;

        // add source code
        SourceDataBlock sourceDataBlock1(FOREACH_TILLING_DEF_H, FOREACH_TILLING_DEF_H_PATH);
        dataFileGenerator.AddDataBlock(std::make_unique<SourceDataBlock>(sourceDataBlock1));
        SourceDataBlock sourceDataBlock2(KERNEL_FOREACH_BASE_H, KERNEL_FOREACH_BASE_H_PATH);
        dataFileGenerator.AddDataBlock(std::make_unique<SourceDataBlock>(sourceDataBlock2));
        SourceDataBlock sourceDataBlock3(FOREACH_SUB_SCALAR_LIST_CPP, FOREACH_SUB_SCALAR_LIST_CPP_PATH);
        dataFileGenerator.AddDataBlock(std::make_unique<SourceDataBlock>(sourceDataBlock3));
        // add api instructions
        NormalDataBlock apiInstrDataBlock(DataTypeEnum::API_INSTR, API_INSTR_JSON);
        dataFileGenerator.AddDataBlock(std::make_unique<NormalDataBlock>(apiInstrDataBlock));
        // add api file
        NormalDataBlock apiFileDataBlock(DataTypeEnum::API_FILE, API_FILE_JSON);
        dataFileGenerator.AddDataBlock(std::make_unique<NormalDataBlock>(apiFileDataBlock));
        // generate file
        dataFileGenerator.Generate(data);

        string currPath = Dic::FileUtil::GetCurrPath();
        int index = currPath.find_last_of("server");
        currPath = currPath.substr(0, index + 1);
        memoryData = currPath + R"(/src/test/test_data/memory_data.bin)";
        baseInfo = currPath + R"(/src/test/test_data/base_info.bin)";
        loadData = currPath + R"(/src/test/test_data/load_data.bin)";
        rooflineDataFile = currPath + R"(/src/test/test_data/roofline_data.bin)";
    }

    void TearDown() override
    {
        // remove temprary bin files
        BinFileGenerator::RemoveFile(data);
    }
};

TEST_F(SourceTest, CheckOperatorBinarySuccessfulWithFixedPath)
{
    Module::Source::SourceFileParser &parser = Dic::Module::Source::SourceFileParser::Instance();
    EXPECT_EQ(true, parser.CheckOperatorBinary(data));
    EXPECT_EQ(true, parser.CheckOperatorBinary(memoryData));
    EXPECT_EQ(true, parser.CheckOperatorBinary(baseInfo));
}

TEST_F(SourceTest, CheckOperatorBinaryFailedWithSpecificPath)
{
    Module::Source::SourceFileParser &parser = Dic::Module::Source::SourceFileParser::Instance();
#ifdef _WIN32
    const int pathLen = MAX_PATH;
#else
    const int pathLen = PATH_MAX;
#endif
    string path;
    for (int i = 0; i < pathLen - 1; i++) {
        path += "0";
    }
    // Failed to check file exists
    EXPECT_EQ(false, parser.CheckOperatorBinary(path));

    // Failed to check file length
    path += "0";
    EXPECT_EQ(false, parser.CheckOperatorBinary(path));
}

static Module::Source::SourceFileParser &InitParser(const string& dataPath)
{
    Module::Source::SourceFileParser &parser = Dic::Module::Source::SourceFileParser::Instance();
    EXPECT_EQ(true, parser.CheckOperatorBinary(dataPath));
    parser.SetFilePath(dataPath);
    parser.Parse(std::vector<std::string>(), "", dataPath);
    parser.ConvertToData();
    return parser;
}

static void CleanParser(Module::Source::SourceFileParser &parser)
{
    parser.Reset();
}


TEST_F(SourceTest, GetCoreList)
{
    auto &parser = InitParser(data);
    const vector<std::string> &coreList = parser.GetCoreList();
    int coreListSize = 3;
    EXPECT_EQ(coreList.size(), coreListSize);
    EXPECT_EQ(coreList[0], "core0.veccore0");
    EXPECT_EQ(coreList[1], "core0.veccore1");
    EXPECT_EQ(coreList[2], "core1.veccore0"); // the core index is 2
    CleanParser(parser);
}

TEST_F(SourceTest, GetSourceList)
{
    auto &parser = InitParser(data);
    const vector<std::string> &sourceList = parser.GetSourceList();
    int sourceListSize = 3;
    EXPECT_EQ(sourceList.size(), sourceListSize);
    EXPECT_EQ(sourceList[0], FOREACH_TILLING_DEF_H_PATH);
    EXPECT_EQ(sourceList[1], KERNEL_FOREACH_BASE_H_PATH);
    EXPECT_EQ(sourceList[2], FOREACH_SUB_SCALAR_LIST_CPP_PATH); // index of scalar list cpp is 2
    CleanParser(parser);
}


TEST_F(SourceTest, GetApiLinesByCoreAndSource)
{
    auto &parser = InitParser(data);
    vector<Dic::Module::Source::SourceFileLine> apiLines = parser.GetApiLinesByCoreAndSource(
        "core0.cubecore",
        "/home/test/workspace/foreach/common/foreach/foreach_tiling_def.h");
    EXPECT_EQ(apiLines.size(), 0);

    apiLines = parser.GetApiLinesByCoreAndSource(
        "core0.veccore0",
        "/home/test/workspace/foreach/common/foreach/foreach_tiling.h");
    EXPECT_EQ(apiLines.size(), 0);

    const vector<Dic::Module::Source::SourceFileLine> &linesByCoreAndSource = parser.GetApiLinesByCoreAndSource(
        
        "core0.veccore0", "/home/test/workspace/foreach/common/foreach/foreach_tiling_def.h");
    int linesByCoreAndSourceSize = 2;
    EXPECT_EQ(linesByCoreAndSource.size(), linesByCoreAndSourceSize);
    EXPECT_EQ(31, linesByCoreAndSource[0].line); // the line is 31
    EXPECT_EQ(15674, linesByCoreAndSource[0].cycles[0]); // the size of cycles is 15674
    EXPECT_EQ(192, linesByCoreAndSource[0].instructionsExecuted[0]); // the size of instructions is 192
    EXPECT_EQ("0x11244240", linesByCoreAndSource[0].addressRange[0].first);
    EXPECT_EQ("0x11244240", linesByCoreAndSource[0].addressRange[0].second);

    EXPECT_EQ(32, linesByCoreAndSource[1].line); // the size of line is 32 and the index is 1
    EXPECT_EQ(16293, linesByCoreAndSource[1].cycles[0]); // the size of cycles is 16293
    EXPECT_EQ(193, linesByCoreAndSource[1].instructionsExecuted[0]); // the size of instructions is 193
    EXPECT_EQ("0x11244540", linesByCoreAndSource[1].addressRange[0].first); // the index of line is 1
    EXPECT_EQ("0x11244540", linesByCoreAndSource[1].addressRange[0].second); // the index of line is 1
    EXPECT_EQ("0x11244548", linesByCoreAndSource[1].addressRange[1].first); // the index of line is 1
    EXPECT_EQ("0x11244844", linesByCoreAndSource[1].addressRange[1].second); // the index of line is 1
    CleanParser(parser);
}

TEST_F(SourceTest, GetInstr)
{
    auto &parser = InitParser(data);
    const string &instr = parser.GetInstr();
    EXPECT_TRUE(!instr.empty());
    EXPECT_EQ(true, StringUtil::Contains(instr, "Cores"));
    EXPECT_EQ(true, StringUtil::Contains(instr, "core0.veccore0"));
    EXPECT_EQ(true, StringUtil::Contains(instr, "Address"));
    EXPECT_EQ(true, StringUtil::Contains(instr, "Instructions Executed"));
    CleanParser(parser);
}

TEST_F(SourceTest, GetSourceByName)
{
    auto &parser = InitParser(data);
    std::string foreachTillingPath = "/home/test/workspace/foreach/common/foreach/foreach_tiling.h";
    const string notExistsource = parser.GetSourceByName(foreachTillingPath);
    EXPECT_TRUE(notExistsource.empty());

    std::string foreachTillingDefPath = "/home/test/workspace/foreach/common/foreach/foreach_tiling_def.h";
    const string &source = parser.GetSourceByName(foreachTillingDefPath);
    EXPECT_TRUE(!source.empty());

    EXPECT_EQ(true, StringUtil::Contains(source, "__FOREACH_TILING_DEF_H__"));
    EXPECT_EQ(true, StringUtil::Contains(source, "ForeachCommonTilingData"));
    EXPECT_EQ(true, StringUtil::Contains(source, "endif"));
    CleanParser(parser);
}


TEST_F(SourceTest, QueryBaseInfo)
{
    auto &parser = InitParser(baseInfo);
    Protocol::DetailsBaseInfoResBody resBody;
    bool res = parser.GetDetailsBaseInfo(resBody, false);
    EXPECT_EQ(true, res);
    EXPECT_EQ("sin_custom", resBody.name);
    EXPECT_EQ("Ascend910B4", resBody.soc);
    EXPECT_EQ("32", resBody.blockDim);
    EXPECT_EQ("13.0600004196167", resBody.duration);
    parser.Reset();
}

TEST_F(SourceTest, GetDetailsLoadInfo)
{
    auto &parser = InitParser(loadData);
    Protocol::DetailsLoadInfoResBody responseBody;
    bool res = parser.GetDetailsLoadInfo(responseBody, false);
    EXPECT_EQ(true, res);
    EXPECT_EQ(32, responseBody.blockIdList.size()); // block id list is 32
    parser.Reset();
}

TEST_F(SourceTest, GetDetailsMemoryGraph)
{
    auto &parser = InitParser(memoryData);
    Protocol::DetailsMemoryGraphResBody resBody;
    bool res = parser.GetDetailsMemoryGraph("0", false, resBody);
    EXPECT_EQ(true, res);
    parser.Reset();
}

TEST_F(SourceTest, GetDetailsMemoryTable)
{
    auto &parser = InitParser(memoryData);
    Protocol::DetailsMemoryTableResBody resBody;
    bool res = parser.GetDetailsMemoryTable("", false, resBody);
    EXPECT_EQ(true, res);
    EXPECT_EQ(0, resBody.memoryTable.size());

    res = parser.GetDetailsMemoryTable("32", false, resBody);
    EXPECT_EQ(true, res);
    EXPECT_EQ(0, resBody.memoryTable.size());

    res = parser.GetDetailsMemoryTable("0", false, resBody);
    EXPECT_EQ(true, res);
    EXPECT_EQ(1, resBody.memoryTable.size());
    parser.Reset();
}

TEST_F(SourceTest, GetRoofline)
{
    auto &parser = InitParser(rooflineDataFile);
    Protocol::DetailsRooflineBody resBody;
    bool res = parser.GetDetailsRoofline(resBody);
    EXPECT_EQ(true, res);
    EXPECT_EQ(resBody.soc, "Ascend910B4");
    int dataSize = 7;
    EXPECT_EQ(resBody.data.size(), dataSize);
    auto item = resBody.data[0];
    EXPECT_EQ(item.title, "Memory Unit(Cube)");
    int rooflineSize = 6;
    EXPECT_EQ(item.rooflines.size(), rooflineSize);
    std::string expect = "18.440187454223633";
    EXPECT_EQ(item.rooflines[0].bw, expect);
    parser.Reset();
}