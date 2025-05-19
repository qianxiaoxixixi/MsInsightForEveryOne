/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <gtest/gtest.h>
#include "QueryTableDataDetailHandler.h"
#include "QueryTableDataNameListHandler.h"
#include "../../../DatabaseTestCaseMockUtil.cpp"
#include "HandlerTest.cpp"

TEST_F(HandlerTest, QueryTableDataDetailHandlerTestNormal)
{
    Dic::Module::Timeline::QueryTableDataDetailHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::TableDataDetailRequest>();
    auto res = handler.HandleRequest(std::move(requestPtr));
    EXPECT_EQ(res, false);
}

TEST_F(HandlerTest, QueryTableDataNameListHandlerTestNormal)
{
    Dic::Module::Timeline::QueryTableDataNameListHandler handler;
    std::unique_ptr<Dic::Protocol::Request> requestPtr = std::make_unique<Dic::Protocol::TableDataNameListRequest>();
    auto res = handler.HandleRequest(std::move(requestPtr));
    EXPECT_EQ(res, false);
}

TEST_F(HandlerTest, ComputeTableDetailTestNormal)
{
    Dic::Module::Timeline::QueryTableDataDetailHandler handler;
    TableDataDetailRequest request;
    TableDataDetailResponse response;
    std::recursive_mutex sqlMutex;
    std::shared_ptr<MockDatabase> vDb = std::make_shared<MockDatabase>(sqlMutex);
    sqlite3* dbPtr = nullptr;
    Dic::Global::PROFILER::MockUtil::DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    std::string sql1 = "CREATE TABLE \"data_table\" (\n"
                       "\"id\" INTEGER NOT NULL,\n"
                       "\"name\" TEXT,\n"
                       "\"view_name\" TEXT,\n"
                       "PRIMARY KEY (\"id\")\n"
                       ");";
    std::string sql2 =
        "INSERT INTO \"main\".\"data_table\" (\"id\", \"name\", \"view_name\") VALUES (2, 'kvcache', 'kvcache');";
    std::string sql3 = "CREATE TABLE \"kvcache\" (\n"
                       "\"rid\" TEXT,\n"
                       "  \"name\" TEXT,\n"
                       "  \"real_start_time\" TEXT,\n"
                       "  \"device_kvcache_left\" REAL,\n"
                       "  \"kvcache_usage_rate\" REAL\n"
                       ");";
    std::string sql4 =
        "INSERT INTO \"main\".\"kvcache\" (\"rid\", \"name\", \"real_start_time\", \"device_kvcache_left\", "
        "\"kvcache_usage_rate\") VALUES ('0', 'Allocate', '2025-03-28 22:57:43:410383', 1217.0, "
        "0.000821018062397373);\n"
        "INSERT INTO \"main\".\"kvcache\" (\"rid\", \"name\", \"real_start_time\", \"device_kvcache_left\", "
        "\"kvcache_usage_rate\") VALUES ('1', 'Allocate', '2025-03-28 22:57:43:410414', 1217.0, 0.000821018062397373);";
    Dic::Global::PROFILER::MockUtil::DatabaseTestCaseMockUtil::InsertData(dbPtr, sql1);
    Dic::Global::PROFILER::MockUtil::DatabaseTestCaseMockUtil::InsertData(dbPtr, sql2);
    Dic::Global::PROFILER::MockUtil::DatabaseTestCaseMockUtil::InsertData(dbPtr, sql3);
    Dic::Global::PROFILER::MockUtil::DatabaseTestCaseMockUtil::InsertData(dbPtr, sql4);
    vDb->SetDbPtr(dbPtr);
    request.params.tableIndex = 0;  // 0
    request.params.currentPage = 1;  // 1
    request.params.pageSize = 10;  // 10
    handler.ComputeTableDetail(request, response, vDb);
    EXPECT_EQ(response.body.columnAttr.size(), 5);  // 5
    EXPECT_EQ(response.body.columnData.size(), 2);  // 2
    EXPECT_EQ(response.body.totalNum, 2);  // 2
}
