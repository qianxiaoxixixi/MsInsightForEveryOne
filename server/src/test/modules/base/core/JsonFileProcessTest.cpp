/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include "FileParser.h"
#include "JsonFileProcess.h"

class JsonFileProcessTest : public ::testing::Test {
protected:
    class JsonFileProcessMock : protected Dic::Module::JsonFileProcess {
    public:
        static std::vector<std::pair<int64_t, int64_t>> SplitFileMock(const std::string &filePath)
        {
            return SplitFile(filePath);
        }
    };
    const std::string tempFileName = "temp_test_file.json";
    void TearDown()
    {
        DeleteTempFile(tempFileName);
    }
    std::string GenerateLargeJsonArray(size_t sizeInBytes)
    {
        const uint64_t offset = 10;
        std::string jsonArray = "[";
        while (jsonArray.size() < sizeInBytes - offset) {
            jsonArray += "{\"ph\": \"Xvalue\"},";
        }
        if (jsonArray.back() == ',') {
            jsonArray.back() = ']';
        } else {
            jsonArray += ']';
        }
        return jsonArray;
    }

    std::string GenerateLargeJsonDocument(size_t sizeInBytes)
    {
        const uint64_t offset = 2;
        std::string jsonDocument = "{\"traceEvents\":[";
        std::string jsonObject = "{\"id\": 1, \"ph\": \"XXtest\"}";
        size_t jsonObjectSize = jsonObject.size();
        while (jsonDocument.size() + jsonObjectSize < sizeInBytes - offset) { // 留出空间用于最后的 "]}"
            jsonDocument += jsonObject + ",";
        }
        if (jsonDocument.back() == ',') {
            jsonDocument.back() = ']';
        } else {
            jsonDocument += ']';
        }
        jsonDocument += "}";

        return jsonDocument;
    }

    std::string GenerateWrongEndJsonDocument(size_t sizeInBytes)
    {
        const uint64_t offset = 2;
        std::string jsonDocument = "{}]}\"traceEvents\":[";
        std::string jsonObject = "{\"id\": 1, \"name\": \"test\"}";
        size_t jsonObjectSize = jsonObject.size();
        while (jsonDocument.size() + jsonObjectSize < sizeInBytes - offset) {
            jsonDocument += jsonObject + ",";
        }
        jsonDocument += "}";

        return jsonDocument;
    }

    std::string GenerateWrongStartJsonDocument(size_t sizeInBytes)
    {
        const uint64_t offset = 2;
        std::string jsonDocument = "{\"traceEvents\":@@@";
        std::string jsonObject = "{\"id\": 1, \"name\": \"test\"}";
        size_t jsonObjectSize = jsonObject.size();
        while (jsonDocument.size() + jsonObjectSize < sizeInBytes - offset) {
            jsonDocument += jsonObject + ",";
        }
        if (jsonDocument.back() == ',') {
            jsonDocument.back() = ']';
        } else {
            jsonDocument += ']';
        }
        jsonDocument += "}";

        return jsonDocument;
    }

    std::string WriteTempJsonFile(const std::string &data)
    {
        std::ofstream tempFile(tempFileName, std::ios::out | std::ios::trunc);
        if (tempFile.is_open()) {
            tempFile << data;
            tempFile.close();
        } else {
            throw std::runtime_error("Failed to create temporary file.");
        }
        return tempFileName;
    }

    // 删除临时文件
    void DeleteTempFile(const std::string &fileName)
    {
        if (std::remove(fileName.c_str()) != 0) {
            std::cerr << "Error deleting temporary file." << std::endl;
        }
    }
};

/**
 * 测试文件打开失败的情况
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenFileIsNotOpen)
{
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 0;
    EXPECT_EQ(res.size(), expectSize);
}

/**
 * 测试0.5kb的json数组
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonArrayFileIs500ByteThenReturn0and0)
{
    size_t sizeInBytes = 1024 / 2;
    std::string json = GenerateLargeJsonArray(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 1;
    const uint64_t expectStart = 0;
    const uint64_t expectEnd = 0;
    const uint64_t first = 0;
    EXPECT_EQ(res.size(), expectSize);
    EXPECT_EQ(res[first].first, expectStart);
    EXPECT_EQ(res[first].second, expectEnd);
}

/**
 * 测试0.5kb的json对象
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonObjectFileIs500ByteThenReturnOneSplit)
{
    size_t sizeInBytes = 1024 / 2;
    std::string json = GenerateLargeJsonDocument(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 1;
    const uint64_t expectStart = 16;
    const uint64_t expectEnd = 508;
    const uint64_t first = 0;
    EXPECT_EQ(res.size(), expectSize);
    EXPECT_EQ(res[first].first, expectStart);
    EXPECT_EQ(res[first].second, expectEnd);
}

/**
 * 测试0.5kb的json对象没有开始位置
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonObjectFileWithOutStartMarkThenReturnZeroSplit)
{
    size_t sizeInBytes = 1024 / 2;
    std::string json = GenerateWrongStartJsonDocument(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 0;
    EXPECT_EQ(res.size(), expectSize);
}

/**
 * 测试0.5kb的json对象没有开始位置
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonObjectFileWithWrongEndMarkThenReturnZeroSplit)
{
    size_t sizeInBytes = 1024 / 2;
    std::string json = GenerateWrongEndJsonDocument(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 0;
    EXPECT_EQ(res.size(), expectSize);
}

/**
 * 测试9kb的json数组
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonArrayFileIs9KBThenReturn0and0)
{
    size_t sizeInBytes = 1024 * 9;
    std::string json = GenerateLargeJsonArray(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 1;
    const uint64_t expectStart = 0;
    const uint64_t expectEnd = 0;
    const uint64_t first = 0;
    EXPECT_EQ(res.size(), expectSize);
    EXPECT_EQ(res[first].first, expectStart);
    EXPECT_EQ(res[first].second, expectEnd);
}

/**
 * 测试9kb的json对象
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonObjectFileIs9KBThenReturnReturnOneSplit)
{
    size_t sizeInBytes = 1024 * 9;
    std::string json = GenerateLargeJsonDocument(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 1;
    const uint64_t expectStart = 16;
    const uint64_t expectEnd = 9192;
    const uint64_t first = 0;
    EXPECT_EQ(res.size(), expectSize);
    EXPECT_EQ(res[first].first, expectStart);
    EXPECT_EQ(res[first].second, expectEnd);
}


/**
 * 测试52MB的json对象
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonObjectFileIs52MBThenReturnReturnTwoSplit)
{
    size_t sizeInBytes = 1024 * 1024 * 52;
    std::string json = GenerateLargeJsonDocument(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 2;
    const uint64_t expectStart = 16;
    const uint64_t expectEnd = 52428832;
    const uint64_t expectSecondStart = 52428834;
    const uint64_t expectSecondEnd = 54525940;
    const uint64_t first = 0;
    const uint64_t second = 1;
    EXPECT_EQ(res.size(), expectSize);
    EXPECT_EQ(res[first].first, expectStart);
    EXPECT_EQ(res[first].second, expectEnd);
    EXPECT_EQ(res[second].first, expectSecondStart);
    EXPECT_EQ(res[second].second, expectSecondEnd);
}

/**
 * 测试52MB的json数组
 */
TEST_F(JsonFileProcessTest, TestSplitFileWhenJsonArrayFileIs52MBThenReturnReturnTwoSplit)
{
    size_t sizeInBytes = 1024 * 1024 * 52;
    std::string json = GenerateLargeJsonArray(sizeInBytes);
    WriteTempJsonFile(json);
    std::vector<std::pair<int64_t, int64_t>> res = JsonFileProcessMock::SplitFileMock(tempFileName);
    const uint64_t expectSize = 2;
    const uint64_t expectStart = 1;
    const uint64_t expectEnd = 52428815;
    const uint64_t expectSecondStart = 52428817;
    const uint64_t expectSecondEnd = 54525952;
    const uint64_t first = 0;
    const uint64_t second = 1;
    EXPECT_EQ(res.size(), expectSize);
    EXPECT_EQ(res[first].first, expectStart);
    EXPECT_EQ(res[first].second, expectEnd);
    EXPECT_EQ(res[second].first, expectSecondStart);
    EXPECT_EQ(res[second].second, expectSecondEnd);
}
