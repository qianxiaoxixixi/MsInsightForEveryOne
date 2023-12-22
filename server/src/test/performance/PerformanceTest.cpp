/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "PerformanceTest.h"
#include <fstream>

namespace Performance {
CaseExecuteStatus PerformanceTest::status = {};
int PerformanceTest::number = 0;

void PerformanceTest::SetUpTestCase()
{
    status = {"No.", "Module", "CaseName", "Result", "Baseline(ms)", "Real Time(ms)"};
    WriteTestCaseResult(status, true);
    number = 0;
}

void PerformanceTest::TearDownTestCase() {
}


int PerformanceTest::Main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

void PerformanceTest::WriteTestCaseResult(const CaseExecuteStatus& result, bool clear)
{
    std::ofstream outfile;
    outfile.open(TEST_RESULT_FILE_NAME, std::ofstream::out | (clear ? std::ofstream::trunc : std::ofstream::app));

    outfile << result.id << SEPARATOR;
    outfile << result.module << SEPARATOR;
    outfile << result.caseName << SEPARATOR;
    outfile << result.result << SEPARATOR;
    outfile << result.refTime << SEPARATOR;
    outfile << result.realTime << std::endl;
    outfile.close();
}

std::string PerformanceTest::GetNumber()
{
    return std::to_string(++number);
}

void PerformanceTest::SetUp()
{
    Test::SetUp();
    status.id = GetNumber();
    status.module = "";
    status.caseName = "";
    status.result = "failed";
    status.refTime = "";
    status.realTime = "";
}

void PerformanceTest::TearDown()
{
    status.result = ::testing::UnitTest::GetInstance()->ad_hoc_test_result().Passed() ? "pass" : "failed";
    WriteTestCaseResult(status, false);
    Test::TearDown();
}

}

