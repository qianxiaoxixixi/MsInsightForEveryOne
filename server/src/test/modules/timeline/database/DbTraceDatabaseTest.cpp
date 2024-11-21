/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "DbTraceDataBase.h"
#include "DataBaseManager.h"
#include "../../../DatabaseTestCaseMockUtil.cpp"
using namespace Dic::Global::PROFILER::MockUtil;
class DbTraceDatabaseTest : public ::testing::Test {};
namespace Dic::Protocol {
    using namespace Dic::Module::Timeline;
}

/**
 * RANK_DEVICE_MAP表和NPU_INFO都不存在的情况
 */
TEST_F(DbTraceDatabaseTest, TestQueryRankIdWithRankDeviceAndNpuInfoTableNotExist)
{
    Dic::Protocol::DataBaseManager::Instance().Clear();
    std::recursive_mutex testMutex;
    Dic::Module::FullDb::DbTraceDataBase database(testMutex);
    std::vector<std::string> rankIds = database.QueryRankId();
    EXPECT_EQ(rankIds.size(), 0);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * RANK_DEVICE_MAP表存在的情况
 */
TEST_F(DbTraceDatabaseTest, TestQueryRankIdWithRankDeviceExist)
{
    class MockDatabase : public Dic::Module::FullDb::DbTraceDataBase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : DbTraceDataBase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            return;
        }
    };
    Dic::Protocol::DataBaseManager::Instance().Clear();
    Dic::Protocol::DataBaseManager::Instance().SetFileType(Dic::Protocol::FileType::PYTORCH);
    std::recursive_mutex testMutex;
    MockDatabase database(testMutex);
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE RANK_DEVICE_MAP (rankId INTEGER, deviceId INTEGER);";
    DatabaseTestCaseMockUtil::OpenDBAndCreateTable(db, sql);
    std::string insertSql = "INSERT INTO RANK_DEVICE_MAP (rankId, deviceId) VALUES (999, 8), (276878, 7);";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    database.SetDbPtr(db);
    std::vector<std::string> rankIds = database.QueryRankId();
    const uint32_t expectSize = 2;
    const std::string firstRankId = "999";
    const std::string secondRankId = "276878";
    const uint32_t first = 0;
    const uint32_t second = 1;
    EXPECT_EQ(rankIds.size(), expectSize);
    EXPECT_EQ(rankIds[first], firstRankId);
    EXPECT_EQ(rankIds[second], secondRankId);
    rankIds = database.QueryRankId();
    EXPECT_EQ(rankIds.size(), expectSize);
    EXPECT_EQ(rankIds[first], firstRankId);
    EXPECT_EQ(rankIds[second], secondRankId);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * RANK_DEVICE_MAP表不存在但NPU_INFO存在的情况
 */
TEST_F(DbTraceDatabaseTest, TestQueryRankIdWithNpuInfoExist)
{
    class MockDatabase : public Dic::Module::FullDb::DbTraceDataBase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : DbTraceDataBase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            return;
        }
    };
    Dic::Protocol::DataBaseManager::Instance().Clear();
    std::recursive_mutex testMutex;
    MockDatabase database(testMutex);
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE NPU_INFO (id INTEGER, name TEXT);";
    Dic::Protocol::DataBaseManager::Instance().SetFileType(Dic::Protocol::FileType::MS_PROF);
    DatabaseTestCaseMockUtil::OpenDBAndCreateTable(db, sql);
    std::string insertSql = "INSERT INTO NPU_INFO (id, name) VALUES (3, 'h'), (4, 'kk');";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    database.SetDbPtr(db);
    std::vector<std::string> rankIds = database.QueryRankId();
    const uint32_t expectSize = 2;
    const std::string firstRankId = "3";
    const std::string secondRankId = "4";
    const uint32_t first = 0;
    const uint32_t second = 1;
    EXPECT_EQ(rankIds.size(), expectSize);
    EXPECT_EQ(rankIds[first], firstRankId);
    EXPECT_EQ(rankIds[second], secondRankId);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * QueryRankIdAndDeviceMap函数RANK_DEVICE_MAP表和NPU_INFO都不存在的情况
 */
TEST_F(DbTraceDatabaseTest, TestQueryRankIdAndDeviceMapWithRankDeviceAndNpuInfoTableNotExist)
{
    Dic::Protocol::DataBaseManager::Instance().Clear();
    std::recursive_mutex testMutex;
    Dic::Module::FullDb::DbTraceDataBase database(testMutex);
    std::unordered_map<std::string, std::string> rankIds = database.QueryRankIdAndDeviceMap();
    EXPECT_EQ(rankIds.size(), 0);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * QueryRankIdAndDeviceMap函数RANK_DEVICE_MAP表存在的情况
 */
TEST_F(DbTraceDatabaseTest, TestQueryRankIdAndDeviceMapWithRankDeviceExist)
{
    class MockDatabase : public Dic::Module::FullDb::DbTraceDataBase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : DbTraceDataBase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            return;
        }
    };
    Dic::Protocol::DataBaseManager::Instance().Clear();
    Dic::Protocol::DataBaseManager::Instance().SetFileType(Dic::Protocol::FileType::PYTORCH);
    std::recursive_mutex testMutex;
    MockDatabase database(testMutex);
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE RANK_DEVICE_MAP (rankId INTEGER, deviceId INTEGER);";
    DatabaseTestCaseMockUtil::OpenDBAndCreateTable(db, sql);
    std::string insertSql = "INSERT INTO RANK_DEVICE_MAP (rankId, deviceId) VALUES (999, 8), (276878, 7);";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    database.SetDbPtr(db);
    std::unordered_map<std::string, std::string> rankIds = database.QueryRankIdAndDeviceMap();
    const uint32_t expectSize = 2;
    const std::string firstRankId = "999";
    const std::string secondRankId = "276878";
    const std::string first = "8";
    const std::string second = "7";
    EXPECT_EQ(rankIds.size(), expectSize);
    EXPECT_EQ(rankIds[firstRankId], first);
    EXPECT_EQ(rankIds[secondRankId], second);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * QueryRankIdAndDeviceMap函数RANK_DEVICE_MAP表存在但列名错误
 */
TEST_F(DbTraceDatabaseTest, TestQueryRankIdAndDeviceMapWithRankDeviceWrongColumn)
{
    class MockDatabase : public Dic::Module::FullDb::DbTraceDataBase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : DbTraceDataBase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            return;
        }
    };
    Dic::Protocol::DataBaseManager::Instance().Clear();
    Dic::Protocol::DataBaseManager::Instance().SetFileType(Dic::Protocol::FileType::PYTORCH);
    std::recursive_mutex testMutex;
    MockDatabase dataBase(testMutex);
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE RANK_DEVICE_MAP (rankId INTEGER, devicId INTEGER);";
    DatabaseTestCaseMockUtil::OpenDBAndCreateTable(db, sql);
    std::string insertSql = "INSERT INTO RANK_DEVICE_MAP (rankId, devicId) VALUES (999, 8), (276878, 7);";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    dataBase.SetDbPtr(db);
    std::unordered_map<std::string, std::string> rankIds = dataBase.QueryRankIdAndDeviceMap();
    EXPECT_EQ(rankIds.size(), 0);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * GetDeviceId函数host存在
 */
TEST_F(DbTraceDatabaseTest, TestGetDeviceIdWithHostExist)
{
    class MockDatabase : public Dic::Module::FullDb::DbTraceDataBase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : DbTraceDataBase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            return;
        }
    };
    Dic::Protocol::DataBaseManager::Instance().Clear();
    Dic::Protocol::DataBaseManager::Instance().SetFileType(Dic::Protocol::FileType::PYTORCH);
    std::recursive_mutex testMutex;
    MockDatabase database(testMutex);
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE RANK_DEVICE_MAP (rankId INTEGER, deviceId INTEGER);";
    DatabaseTestCaseMockUtil::OpenDBAndCreateTable(db, sql);
    std::string insertSql = "INSERT INTO RANK_DEVICE_MAP (rankId, deviceId) VALUES (999, 8), (276878, 7);";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    std::string hostSql = "CREATE TABLE HOST_INFO (hostUid TEXT,hostName TEXT);";
    DatabaseTestCaseMockUtil::CreateTable(db, hostSql);
    insertSql =
        "INSERT INTO \"main\".\"HOST_INFO\" (\"hostUid\", \"hostName\") VALUES ('3203900921565068809', 'ubuntu');";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    database.SetDbPtr(db);
    std::string fileId = "ubuntu3203900921565068809 999";
    std::string deviceId = database.GetDeviceId(fileId);
    const std::string expectDeviceId = "8";
    EXPECT_EQ(deviceId, expectDeviceId);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}

/**
 * GetDeviceId函数host不存在
 */
TEST_F(DbTraceDatabaseTest, TestGetDeviceIdWithHostNotExist)
{
    class MockDatabase : public Dic::Module::FullDb::DbTraceDataBase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : DbTraceDataBase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            return;
        }
    };
    Dic::Protocol::DataBaseManager::Instance().Clear();
    Dic::Protocol::DataBaseManager::Instance().SetFileType(Dic::Protocol::FileType::PYTORCH);
    std::recursive_mutex testMutex;
    MockDatabase database(testMutex);
    sqlite3 *db = nullptr;
    std::string sql = "CREATE TABLE RANK_DEVICE_MAP (rankId INTEGER, deviceId INTEGER);";
    DatabaseTestCaseMockUtil::OpenDBAndCreateTable(db, sql);
    std::string insertSql = "INSERT INTO RANK_DEVICE_MAP (rankId, deviceId) VALUES (999, 8), (276878, 7);";
    DatabaseTestCaseMockUtil::InsertData(db, insertSql);
    database.SetDbPtr(db);
    std::string fileId = "276878";
    std::string deviceId = database.GetDeviceId(fileId);
    const std::string expectDeviceId = "7";
    EXPECT_EQ(deviceId, expectDeviceId);
    Dic::Protocol::DataBaseManager::Instance().Clear();
}