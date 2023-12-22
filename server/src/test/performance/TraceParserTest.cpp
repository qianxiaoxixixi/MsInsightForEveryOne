/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include <gtest/gtest.h>
#include "PerformanceTest.h"
#include "FileUtil.h"
#include "DataBaseManager.h"
#include "TraceFileParser.h"
#include "ParserStatusManager.h"

using namespace Performance;
using namespace Dic;
using namespace Dic::Module;
using namespace Dic::Module::Timeline;
const std::string TIMELINE_MODULE = "Timeline";
const int TIME_REFERENCE_5S = 5000;

class TraceParserTest : PerformanceTest {
};

TEST_F(PerformanceTest, testTraceParser1P100MTime)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::string currPath = Dic::FileUtil::GetCurrPath();
    std::string rootPath = currPath.substr(0, currPath.find_last_of("server") + 1);
    DataBaseManager::Instance().CreatConnectionPool("0",
        rootPath + R"(/src/test/test_data/test_rank_0/ASCEND_PROFILER_OUTPUT/trace_view.db)");
    TraceFileParser::Instance().Parse(
        {rootPath + R"(/src/test/test_data/test_rank_0/ASCEND_PROFILER_OUTPUT/trace_view.json)"}, "0", "");
    while (true) {
        ParserStatus status = ParserStatusManager::Instance().GetParserStatus("0");
        if (status == ParserStatus::FINISH) {
            break;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    EXPECT_LT(cost, TIME_REFERENCE_5S);
    status.module = TIMELINE_MODULE;
    status.caseName = ::testing::UnitTest::GetInstance()->current_test_info()->name();
    status.refTime = std::to_string(TIME_REFERENCE_5S);
    status.realTime = std::to_string(cost);
    TraceFileParser::Instance().Reset();
}