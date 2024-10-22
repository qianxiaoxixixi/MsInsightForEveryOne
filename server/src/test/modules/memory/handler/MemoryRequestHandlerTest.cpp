/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include <WsSessionManager.h>
#include "WsSessionImpl.h"
#include "QueryMemoryOperatorHandler.h"
#include "QueryMemoryResourceTypeHandler.h"
#include "QueryMemoryStaticOperatorGraphHandler.h"
#include "QueryMemoryStaticOperatorListHandler.h"
#include "QueryMemoryTypeHandler.h"
#include "QueryMemoryViewHandler.h"
#include "QueryOperatorSizeHandler.h"
#include "MemoryProtocolRespose.h"
#include "DataBaseManager.h"
#include "DbMemoryDataBase.h"
#include "TraceTime.h"

using namespace Dic::Module::Timeline;
using namespace Dic::Module::Memory;
using namespace Dic::Protocol;
using namespace Dic::Module::FullDb;
using namespace Dic;

class MemoryRequestHandlerTest : public ::testing::Test {
public:
    static void SetUpTestSuite()
    {
        Dic::Server::WsChannel *ws;
        std::unique_ptr<Dic::Server::WsSession> session = std::make_unique<Dic::Server::WsSessionImpl>(ws);
        Dic::Server::WsSessionManager::Instance().AddSession(std::move(session));
        std::string currPath = Dic::FileUtil::GetCurrPath();
        int index = currPath.find("server");
        currPath = currPath.substr(0, index);
        std::string dbPath = R"(test/data/pytorch/db/level1/rank0_ascend_pt/ASCEND_PROFILER_OUTPUT/)";
        DataBaseManager::Instance().SetDataType(DataType::DB);
        DataBaseManager::Instance().SetFileType(FileType::PYTORCH);
        auto memoryDatabase = std::dynamic_pointer_cast<DbMemoryDataBase, VirtualMemoryDataBase>(
            DataBaseManager::Instance().GetMemoryDatabase("0"));
        memoryDatabase->OpenDb(currPath + dbPath + "ascend_pytorch_profiler_0.db", false);
    }
    static void TearDownTestSuite()
    {
        auto session = Dic::Server::WsSessionManager::Instance().GetSession();
        if (session != nullptr) {
            session->SetStatus(Dic::Server::WsSession::Status::CLOSED);
            session->WaitForExit();
            Dic::Server::WsSessionManager::Instance().RemoveSession();
        }
        auto memoryDatabase = DataBaseManager::Instance().GetMemoryDatabase("0");
        memoryDatabase->CloseDb();
        DataBaseManager::Instance().Clear();
    }
};

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerNormalTest)
{
    const int64_t defaultPageSize = 10;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryOperatorRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "Overall";
    requestPtr.get()->params.currentPage = 1;
    requestPtr.get()->params.pageSize = defaultPageSize;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerInvalidParamTest)
{
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryOperatorRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "Overall";
    requestPtr.get()->params.minSize = 1;
    requestPtr.get()->params.maxSize = -1;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerComparisonGroupByStreamTest)
{
    const int64_t defaultPageSize = 10;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorRequest> requestPtr =
            std::make_unique<Dic::Protocol::MemoryOperatorRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "Stream";
    requestPtr.get()->params.currentPage = 1;
    requestPtr.get()->params.pageSize = defaultPageSize;
    requestPtr.get()->params.isCompare = true;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerGetOperatorDiffCompareEmptyTest)
{
    MemoryOperatorResponse compareData;
    MemoryOperatorResponse baselineData;
    MemoryOperatorComparisonResponse diffData;
    std::vector<MemoryOperator> opDetails;
    MemoryOperator operatorFirst = {"aten::empty_strided", 100, "0.000", "0.000", 1000, "0.000",
        1000, 1000, 1000, 1000, 1000, 1000, 1000, "", ""};
    MemoryOperator operatorSecond = {"matmul", 1000, "0.000", "0.000", 25, "0.000",
        25, 25, 25, 25, 25, 25, 25, "", ""};
    opDetails.push_back(operatorFirst);
    opDetails.push_back(operatorSecond);
    baselineData.operatorDetails = opDetails;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorRequest> requestPtr =
            std::make_unique<Dic::Protocol::MemoryOperatorRequest>();
    handler.GetOperatorDiff(compareData, baselineData, diffData);
    ASSERT_EQ(diffData.operatorDiffDetails.size(), opDetails.size());
    for (size_t i = 0; i < opDetails.size() && i < diffData.operatorDiffDetails.size(); ++i) {
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.name, opDetails[i].name);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.size, -opDetails[i].size);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationTime, opDetails[i].allocationTime);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseTime, opDetails[i].releaseTime);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.duration, -opDetails[i].duration);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.activeReleaseTime, opDetails[i].activeReleaseTime);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.activeDuration, -opDetails[i].activeDuration);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationAllocated, -opDetails[i].allocationAllocated);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationReserved, -opDetails[i].allocationReserved);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationActive, -opDetails[i].allocationActive);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseAllocated, -opDetails[i].releaseAllocated);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseReserved, -opDetails[i].releaseReserved);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseActive, -opDetails[i].releaseActive);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.streamId, "");
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.deviceType, "");
    }
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerGetOperatorDiffBaselineEmptyTest)
{
    MemoryOperatorResponse compareData;
    MemoryOperatorResponse baselineData;
    MemoryOperatorComparisonResponse diffData;
    std::vector<MemoryOperator> opDetails;
    MemoryOperator operatorFirst = {"aten::empty_strided", 100, "0.000", "0.000", 1000, "0.000",
        1000, 1000, 1000, 1000, 1000, 1000, 1000, "", ""};
    MemoryOperator operatorSecond = {"matmul", 1000, "0.000", "0.000", 25, "0.000",
        25, 25, 25, 25, 25, 25, 25, "", ""};
    opDetails.push_back(operatorFirst);
    opDetails.push_back(operatorSecond);
    compareData.operatorDetails = opDetails;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorRequest> requestPtr =
            std::make_unique<Dic::Protocol::MemoryOperatorRequest>();
    handler.GetOperatorDiff(compareData, baselineData, diffData);
    ASSERT_EQ(diffData.operatorDiffDetails.size(), opDetails.size());
    for (size_t i = 0; i < opDetails.size() && i < diffData.operatorDiffDetails.size(); ++i) {
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.name, opDetails[i].name);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.size, opDetails[i].size);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationTime, opDetails[i].allocationTime);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseTime, opDetails[i].releaseTime);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.duration, opDetails[i].duration);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.activeReleaseTime, opDetails[i].activeReleaseTime);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.activeDuration, opDetails[i].activeDuration);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationAllocated, opDetails[i].allocationAllocated);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationReserved, opDetails[i].allocationReserved);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.allocationActive, opDetails[i].allocationActive);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseAllocated, opDetails[i].releaseAllocated);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseReserved, opDetails[i].releaseReserved);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.releaseActive, opDetails[i].releaseActive);
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.streamId, "");
        EXPECT_EQ(diffData.operatorDiffDetails[i].diff.deviceType, "");
    }
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerGetOperatorDiffBothNotEmptyTest)
{
    MemoryOperatorResponse compareData;
    MemoryOperatorResponse baselineData;
    MemoryOperatorComparisonResponse diffData;
    std::vector<MemoryOperator> opCompare;
    MemoryOperator operatorCompareFirst = {"aten::empty_strided", 100, "19.235", "21.478", 2, "21.477",
        2, 100, 100, 100, 100, 100, 100, "4589", ""};
    MemoryOperator operatorCompareSecond = {"matmul", 1000, "178.254", "199.375", 20, "199.372",
        20, 1000, 1000, 1000, 1000, 1000, 1000, "2733", ""};
    opCompare.push_back(operatorCompareFirst);
    opCompare.push_back(operatorCompareSecond);
    compareData.operatorDetails = opCompare;
    std::vector<MemoryOperator> opBaseline;
    MemoryOperator operatorBaselineFirst = {"aten::empty_strided", 173, "33.980", "49.211", 16, "49.210",
        16, 173, 173, 173, 173, 173, 173, "3011", ""};
    MemoryOperator operatorBaselineSecond = {"matmul", 477, "1.230", "120.211", 119, "120.210",
        119, 477, 477, 477, 477, 477, 477, "8924", ""};
    opBaseline.push_back(operatorBaselineFirst);
    opBaseline.push_back(operatorBaselineSecond);
    baselineData.operatorDetails = opBaseline;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorRequest> requestPtr =
            std::make_unique<Dic::Protocol::MemoryOperatorRequest>();
    handler.GetOperatorDiff(compareData, baselineData, diffData);
    ASSERT_EQ(diffData.operatorDiffDetails.size(), opCompare.size());
    ASSERT_EQ(diffData.operatorDiffDetails.size(), opBaseline.size());
    for (size_t i = 0; i < diffData.operatorDiffDetails.size(); ++i) {
        const MemoryOperator &result = diffData.operatorDiffDetails[i].diff;
        EXPECT_EQ(result.name, opCompare[i].name);
        EXPECT_EQ(result.name, opBaseline[i].name);
        EXPECT_EQ(result.size, opCompare[i].size - opBaseline[i].size);
        EXPECT_EQ(result.allocationTime, NumberUtil::StringDoubleMinus(opCompare[i].allocationTime,
            opBaseline[i].allocationTime));
        EXPECT_EQ(result.releaseTime, NumberUtil::StringDoubleMinus(opCompare[i].releaseTime,
            opBaseline[i].releaseTime));
        EXPECT_EQ(result.duration, opCompare[i].duration - opBaseline[i].duration);
        EXPECT_EQ(result.activeReleaseTime, NumberUtil::StringDoubleMinus(opCompare[i].activeReleaseTime,
            opBaseline[i].activeReleaseTime));
        EXPECT_EQ(result.activeDuration, opCompare[i].activeDuration - opBaseline[i].activeDuration);
        EXPECT_EQ(result.allocationAllocated, opCompare[i].allocationAllocated - opBaseline[i].allocationAllocated);
        EXPECT_EQ(result.allocationReserved, opCompare[i].allocationReserved - opBaseline[i].allocationReserved);
        EXPECT_EQ(result.allocationActive, opCompare[i].allocationActive - opBaseline[i].allocationActive);
        EXPECT_EQ(result.releaseAllocated, opCompare[i].releaseAllocated - opBaseline[i].releaseAllocated);
        EXPECT_EQ(result.releaseReserved, opCompare[i].releaseReserved - opBaseline[i].releaseReserved);
        EXPECT_EQ(result.releaseActive, opCompare[i].releaseActive - opBaseline[i].releaseActive);
        EXPECT_EQ(result.streamId, "");
        EXPECT_EQ(result.deviceType, "");
    }
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSelectNameAndSizeTest)
{
    MemoryOperatorComparisonResponse fullDiffResult;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opCompareFirst = {"aten::ge", 50, "190.235", "211.478", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opBaselineFirst = {"aten::ge", -150, "186.235", "215.478", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opCompareSecond = {"matmul", 200.25, "25.230", "120.233", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opBaselineSecond = {"matmul", 100.25, "17.650", "95.23", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opCompareThird = {"matmulv3", 18.2, "133.100", "145.257", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opBaselineThird = {"matmulv3", -18.2, "13.100", "148.257", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    opDiffDetails.push_back({opCompareFirst, opBaselineFirst, opDiffFirst});
    opDiffDetails.push_back({opCompareSecond, opBaselineSecond, opDiffSecond});
    opDiffDetails.push_back({opCompareThird, opBaselineThird, opDiffThird});
    fullDiffResult.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.searchName = "matmul";
    const int64_t minSize = -75;
    const int64_t maxSize = 99;
    request.params.minSize = minSize;
    request.params.maxSize = maxSize;
    request.params.startTime = -1;
    request.params.endTime = -1;
    const int defaultPageSize = 10;
    request.params.pageSize = defaultPageSize;
    request.params.currentPage = 1;
    request.params.order = "";
    request.params.orderBy = "";
    std::unique_ptr<MemoryOperatorComparisonResponse> responsePtr =
            std::make_unique<MemoryOperatorComparisonResponse>();
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    bool result = handler.SelectDiffResult(request, responsePtr, fullDiffResult);
    const int expectColumnSize = 15;
    EXPECT_TRUE(result);
    ASSERT_EQ(responsePtr.get()->totalNum, 1);
    EXPECT_EQ(responsePtr.get()->operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(responsePtr.get()->columnAttr.size(), expectColumnSize);
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSelectStartTimeAndEndTimeTest)
{
    MemoryOperatorComparisonResponse fullDiffResult;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opCompareFirst = {"aten::ge", 50, "190.235", "211.478", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opBaselineFirst = {"aten::ge", -150, "186.235", "215.478", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opCompareSecond = {"matmul", 200.25, "25.230", "120.233", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opBaselineSecond = {"matmul", 100.25, "17.650", "95.23", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opCompareThird = {"matmulv3", 18.2, "133.100", "145.257", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opBaselineThird = {"matmulv3", -18.2, "13.100", "148.257", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 0, "", 0, 0, 0, 0, 0, 0, 0, "", ""};
    opDiffDetails.push_back({opCompareFirst, opBaselineFirst, opDiffFirst});
    opDiffDetails.push_back({opCompareSecond, opBaselineSecond, opDiffSecond});
    opDiffDetails.push_back({opCompareThird, opBaselineThird, opDiffThird});
    fullDiffResult.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.searchName = "";
    request.params.minSize = std::numeric_limits<int64_t>::min();
    request.params.maxSize = std::numeric_limits<int64_t>::max();
    const double startTime = 140.012;
    const double endTime = 216.054;
    request.params.startTime = startTime;
    request.params.endTime = endTime;
    const int defaultPageSize = 10;
    request.params.pageSize = defaultPageSize;
    request.params.currentPage = 1;
    request.params.order = "";
    request.params.orderBy = "";
    std::unique_ptr<MemoryOperatorComparisonResponse> responsePtr =
            std::make_unique<MemoryOperatorComparisonResponse>();
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    bool result = handler.SelectDiffResult(request, responsePtr, fullDiffResult);
    const int expectColumnSize = 15;
    const int expectNum = 2;
    EXPECT_TRUE(result);
    ASSERT_EQ(responsePtr.get()->totalNum, expectNum);
    EXPECT_EQ(responsePtr.get()->operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(responsePtr.get()->operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(responsePtr.get()->columnAttr.size(), expectColumnSize);
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSortAscendTestPartOne)
{
    MemoryOperatorComparisonResponse result;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 12.4, "100.253",
        898.74, -25.897, -111.28, 44.9, 18.7, 25.39, -19.36, "1213", "NA2"};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", -10.8, "-15.780",
        -714.28, 14.289, 10.3, -25.1, 19.6, 20.95, 22.3, "1210", "NA1"};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 3, "21.962",
        1.025, 0.213, -20.1, -776.9, 16.1, 27.011, 100, "1212", "NA"};
    opDiffDetails.push_back({{}, {}, opDiffFirst});
    opDiffDetails.push_back({{}, {}, opDiffSecond});
    opDiffDetails.push_back({{}, {}, opDiffThird});
    result.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.order = "ascend";
    const size_t second = 2;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    request.params.orderBy = "name";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
    request.params.orderBy = "size";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "allocation_time";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
    request.params.orderBy = "release_time";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "duration";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "active_release_time";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSortAscendTestPartTwo)
{
    MemoryOperatorComparisonResponse result;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 12.4, "100.253",
                                  898.74, -25.897, -111.28, 44.9, 18.7, 25.39, -19.36, "1213", "NA2"};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", -10.8, "-15.780",
                                   -714.28, 14.289, 10.3, -25.1, 19.6, 20.95, 22.3, "1210", "NA1"};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 3, "21.962",
                                  1.025, 0.213, -20.1, -776.9, 16.1, 27.011, 100, "1212", "NA"};
    opDiffDetails.push_back({{}, {}, opDiffFirst});
    opDiffDetails.push_back({{}, {}, opDiffSecond});
    opDiffDetails.push_back({{}, {}, opDiffThird});
    result.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.order = "ascend";
    const size_t second = 2;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    request.params.orderBy = "active_duration";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "allocation_allocated";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "allocation_reserve";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "allocation_active";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "release_allocated";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "release_reserve";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSortAscendTestPartThree)
{
    MemoryOperatorComparisonResponse result;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 12.4, "100.253",
                                  898.74, -25.897, -111.28, 44.9, 18.7, 25.39, -19.36, "1213", "NA2"};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", -10.8, "-15.780",
                                   -714.28, 14.289, 10.3, -25.1, 19.6, 20.95, 22.3, "1210", "NA1"};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 3, "21.962",
                                  1.025, 0.213, -20.1, -776.9, 16.1, 27.011, 100, "1212", "NA"};
    opDiffDetails.push_back({{}, {}, opDiffFirst});
    opDiffDetails.push_back({{}, {}, opDiffSecond});
    opDiffDetails.push_back({{}, {}, opDiffThird});
    result.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.order = "ascend";
    const size_t second = 2;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    request.params.orderBy = "release_active";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
    request.params.orderBy = "stream";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "device_type";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSortDescendTestPartOne)
{
    MemoryOperatorComparisonResponse result;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 12.4, "100.253",
                                  898.74, -25.897, -111.28, 44.9, 18.7, 25.39, -19.36, "1213", "NA2"};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", -10.8, "-15.780",
                                   -714.28, 14.289, 10.3, -25.1, 19.6, 20.95, 22.3, "1210", "NA1"};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 3, "21.962",
                                  1.025, 0.213, -20.1, -776.9, 16.1, 27.011, 100, "1212", "NA"};
    opDiffDetails.push_back({{}, {}, opDiffFirst});
    opDiffDetails.push_back({{}, {}, opDiffSecond});
    opDiffDetails.push_back({{}, {}, opDiffThird});
    result.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.order = "descend";
    const size_t second = 2;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    request.params.orderBy = "name";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "size";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "allocation_time";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "release_time";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "duration";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "active_release_time";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSortDescendTestPartTwo)
{
    MemoryOperatorComparisonResponse result;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 12.4, "100.253",
                                  898.74, -25.897, -111.28, 44.9, 18.7, 25.39, -19.36, "1213", "NA2"};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", -10.8, "-15.780",
                                   -714.28, 14.289, 10.3, -25.1, 19.6, 20.95, 22.3, "1210", "NA1"};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 3, "21.962",
                                  1.025, 0.213, -20.1, -776.9, 16.1, 27.011, 100, "1212", "NA"};
    opDiffDetails.push_back({{}, {}, opDiffFirst});
    opDiffDetails.push_back({{}, {}, opDiffSecond});
    opDiffDetails.push_back({{}, {}, opDiffThird});
    result.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.order = "descend";
    const size_t second = 2;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    request.params.orderBy = "active_duration";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "allocation_allocated";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "allocation_reserve";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "allocation_active";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
    request.params.orderBy = "release_allocated";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
    request.params.orderBy = "release_reserve";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorHandlerSelectDiffResultSortDescendTestPartThree)
{
    MemoryOperatorComparisonResponse result;
    std::vector<MemoryOperatorComparison> opDiffDetails;
    MemoryOperator opDiffFirst = {"aten::ge", -100, "4.000", "-4.000", 12.4, "100.253",
                                  898.74, -25.897, -111.28, 44.9, 18.7, 25.39, -19.36, "1213", "NA2"};
    MemoryOperator opDiffSecond = {"matmul", 100, "7.58", "25.003", -10.8, "-15.780",
                                   -714.28, 14.289, 10.3, -25.1, 19.6, 20.95, 22.3, "1210", "NA1"};
    MemoryOperator opDiffThird = {"matmulv3", 36.4, "120.000", "-3.000", 3, "21.962",
                                  1.025, 0.213, -20.1, -776.9, 16.1, 27.011, 100, "1212", "NA"};
    opDiffDetails.push_back({{}, {}, opDiffFirst});
    opDiffDetails.push_back({{}, {}, opDiffSecond});
    opDiffDetails.push_back({{}, {}, opDiffThird});
    result.operatorDiffDetails = opDiffDetails;
    MemoryOperatorRequest request;
    request.params.order = "descend";
    const size_t second = 2;
    Dic::Module::Memory::QueryMemoryOperatorHandler handler;
    request.params.orderBy = "release_active";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "aten::ge");
    request.params.orderBy = "stream";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmulv3");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmul");
    request.params.orderBy = "device_type";
    handler.SortResult(request, result);
    EXPECT_EQ(result.operatorDiffDetails[0].diff.name, "aten::ge");
    EXPECT_EQ(result.operatorDiffDetails[1].diff.name, "matmul");
    EXPECT_EQ(result.operatorDiffDetails[second].diff.name, "matmulv3");
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryResourceTypeHandlerNormalTest)
{
    Dic::Module::Memory::QueryMemoryResourceTypeHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryResourceTypeRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryResourceTypeRequest>();
    requestPtr.get()->rankId = "0";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryResourceTypeHandlerInvalidParamTest)
{
    const int invalidLength = 600;
    Dic::Module::Memory::QueryMemoryResourceTypeHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryResourceTypeRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryResourceTypeRequest>();
    std::string invalidRankId(invalidLength, '0');
    requestPtr.get()->rankId = invalidRankId;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryStaticOperatorGraphHandlerNormalTest)
{
    Dic::Module::Memory::QueryMemoryStaticOperatorGraphHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryStaticOperatorGraphRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryStaticOperatorGraphRequest>();
    requestPtr.get()->params.rankId = "0";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryStaticOperatorGraphHandlerInvalidParamTest)
{
    Dic::Module::Memory::QueryMemoryStaticOperatorGraphHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryStaticOperatorGraphRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryStaticOperatorGraphRequest>();
    requestPtr.get()->params.rankId = "";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryStaticOperatorListHandlerNormalTest)
{
    const int64_t defaultPageSize = 10;
    Dic::Module::Memory::QueryMemoryStaticOperatorListHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryStaticOperatorListRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryStaticOperatorListRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.currentPage = 1;
    requestPtr.get()->params.pageSize = defaultPageSize;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryStaticOperatorListHandlerInvalidParamTest)
{
    Dic::Module::Memory::QueryMemoryStaticOperatorListHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryStaticOperatorListRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryStaticOperatorListRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.startNodeIndex = 1;
    requestPtr.get()->params.endNodeIndex = -1;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryTypeHandlerNormalTest)
{
    Dic::Module::Memory::QueryMemoryTypeHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryTypeRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryTypeRequest>();
    requestPtr.get()->rankId = "0";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryTypeHandlerInvalidParamTest)
{
    Dic::Module::Memory::QueryMemoryTypeHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryTypeRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryTypeRequest>();
    requestPtr.get()->rankId = "0&";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryViewHandlerNormalTest)
{
    Dic::Module::Memory::QueryMemoryViewHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryViewRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryViewRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "Overall";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryViewHandlerInvalidParamTest)
{
    Dic::Module::Memory::QueryMemoryViewHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryViewRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryViewRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "Invalid";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorSizeHandlerNormalTest)
{
    Dic::Module::Memory::QueryOperatorSizeHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorSizeRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryOperatorSizeRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "Overall";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(MemoryRequestHandlerTest, QueryMemoryOperatorSizeHandlerInvalidParamTest)
{
    Dic::Module::Memory::QueryOperatorSizeHandler handler;
    std::unique_ptr<Dic::Protocol::MemoryOperatorSizeRequest> requestPtr =
        std::make_unique<Dic::Protocol::MemoryOperatorSizeRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.type = "";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}