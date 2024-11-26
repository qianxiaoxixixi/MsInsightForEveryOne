/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "../../../DatabaseTestCaseMockUtil.cpp"
#include "SliceTable.h"
#include "FlowTable.h"
#include "ThreadTable.h"
#include "ProcessTable.h"
#include "CounterTable.h"
#include "TextTraceDatabase.h"

using namespace Dic::Global::PROFILER::MockUtil;
using namespace Dic::Module::Timeline;
class TextTraceDatabaseMockTest : public ::testing::Test {
protected:
    class MockDatabase : public Dic::Module::Timeline::TextTraceDatabase {
    public:
        explicit MockDatabase(std::recursive_mutex &sqlMutex) : TextTraceDatabase(sqlMutex) {}
        void SetDbPtr(sqlite3 *dbPtr)
        {
            isOpen = true;
            db = dbPtr;
            path = ":memory:";
        }
    };
    SliceTable sliceTable;
    ThreadTable threadTable;
    ProcessTable processTable;
    FlowTable flowTable;
    CounterTable counterTable;
    const std::string sliceTableSql =
        "CREATE TABLE slice (id INTEGER PRIMARY KEY AUTOINCREMENT, timestamp INTEGER, duration INTEGER, name TEXT, "
        "depth INTEGER, track_id INTEGER, cat TEXT, args TEXT, cname TEXT, end_time INTEGER, flag_id TEXT);";
    const std::string threadTableSql = "CREATE TABLE thread (track_id INTEGER PRIMARY KEY, tid TEXT, pid TEXT, "
        "thread_name TEXT, thread_sort_index INTEGER);";
    const std::string flowTableSql = "CREATE TABLE flow (id INTEGER PRIMARY KEY AUTOINCREMENT, flow_id TEXT, name "
        "TEXT, cat TEXT, track_id INTEGER, timestamp INTEGER, type TEXT);";
    const std::string counterTableSql = "CREATE TABLE counter (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, pid "
        "TEXT,timestamp INTEGER, cat TEXT, args TEXT);";
    const std::string processSql =
        "CREATE TABLE process (pid TEXT PRIMARY KEY, process_name TEXT, label TEXT, process_sort_index INTEGER);";
};

/**
 * text场景创建表,如果db未打开，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestCreateTableWhenDbNotOpenThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    Dic::Module::Timeline::TextTraceDatabase database(sqlMutex);
    bool success = database.CreateTable();
    EXPECT_EQ(success, false);
}

/**
 * text场景创建表,如果db打开，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestCreateTableWhenDbOpenThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    bool success = database.CreateTable();
    std::string sliceSql = "INSERT INTO \"main\".\"slice\" (\"id\", \"timestamp\", \"duration\", \"name\", \"depth\", "
        "\"track_id\", \"cat\", \"args\", \"cname\", \"end_time\", \"flag_id\") VALUES (1, "
        "1726717581355878760, 9470, 'Node@launch', NULL, 1, NULL, '{\"Thread "
        "Id\":\"206468\",\"Mode\":\"launch\",\"level\":\"node\",\"id\":\"0\",\"item_id\":\"aclnnCat_"
        "ConcatD_ConcatD\",\"connection_id\":\"63052\"}', '', 1726717581355888230, '');";
    DatabaseTestCaseMockUtil::InsertData(dbPtr, sliceSql);
    EXPECT_EQ(success, true);
    SliceTable sliceTable;
    std::vector<SlicePO> slicepos;
    sliceTable.Select(SliceColumn::TIMESTAMP).ExcuteQuery(dbPtr, slicepos);
    const uint64_t expectTime = 1726717581355878760;
    const uint64_t expectSize = 1;
    EXPECT_EQ(slicepos.size(), expectSize);
    EXPECT_EQ(slicepos[0].timestamp, expectTime);
}

/**
 * text场景删除表,如果db没打开，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestDropTableWhenDbNotOpenThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    Dic::Module::Timeline::TextTraceDatabase database(sqlMutex);
    bool success = database.DropTable();
    EXPECT_EQ(success, false);
}

/**
 * text场景删除表,如果db打开，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestDropTableWhenDbOpenThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    database.CreateTable();
    std::string sliceSql = "INSERT INTO \"main\".\"slice\" (\"id\", \"timestamp\", \"duration\", \"name\", \"depth\", "
        "\"track_id\", \"cat\", \"args\", \"cname\", \"end_time\", \"flag_id\") VALUES (1, "
        "1726717581355878760, 9470, 'Node@launch', NULL, 1, NULL, '{\"Thread "
        "Id\":\"206468\",\"Mode\":\"launch\",\"level\":\"node\",\"id\":\"0\",\"item_id\":\"aclnnCat_"
        "ConcatD_ConcatD\",\"connection_id\":\"63052\"}', '', 1726717581355888230, '');";
    DatabaseTestCaseMockUtil::InsertData(dbPtr, sliceSql);
    bool success = database.DropTable();
    EXPECT_EQ(success, true);
    SliceTable sliceTable;
    std::vector<SlicePO> slicepos;
    sliceTable.Select(SliceColumn::TIMESTAMP).ExcuteQuery(dbPtr, slicepos);
    const uint64_t expectSize = 0;
    EXPECT_EQ(slicepos.size(), expectSize);
}

/**
 * text场景创建索引,如果db没打开，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestCreateIndexWhenDbNotOpenThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    Dic::Module::Timeline::TextTraceDatabase database(sqlMutex);
    bool success = database.CreateIndex();
    EXPECT_EQ(success, false);
}

/**
 * text场景创建索引,如果db打开，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestCreateIndexWhenDbOpenThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    bool success = database.CreateIndex();
    EXPECT_EQ(success, true);
}

/**
 * text场景插入1000条数据，数据库里有1000条
 */
TEST_F(TextTraceDatabaseMockTest, TestInsertSliceWhenInsert1000SliceThenDbHave1000count)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    database.SetDbPtr(dbPtr);
    for (int i = 0; i < CACHE_SIZE; ++i) {
        Trace::Slice event;
        event.name = "hhhhhhhh";
        database.InsertSlice(event);
    }
    std::vector<SlicePO> slicePOS;
    sliceTable.Select(SliceColumn::NAME).ExcuteQuery(dbPtr, slicePOS);
    EXPECT_EQ(slicePOS.size(), CACHE_SIZE);
}

/**
 * text场景插入999条数据，数据库里有0条
 */
TEST_F(TextTraceDatabaseMockTest, TestInsertSliceWhenInsert999SliceThenDbHave0count)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    database.SetDbPtr(dbPtr);
    for (int i = 0; i < CACHE_SIZE - 1; ++i) {
        Trace::Slice event;
        event.name = "hhhhhhhh";
        database.InsertSlice(event);
    }
    std::vector<SlicePO> slicePOS;
    sliceTable.Select(SliceColumn::NAME).ExcuteQuery(dbPtr, slicePOS);
    EXPECT_EQ(slicePOS.size(), 0);
}

/**
 * text场景插入flow1000条数据，数据库里有1000条
 */
TEST_F(TextTraceDatabaseMockTest, TestInsertSliceWhenInsert1000FlowThenDbHave1000count)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    database.SetDbPtr(dbPtr);
    const std::string expectName = "hhhhh";
    for (int i = 0; i < CACHE_SIZE; ++i) {
        Trace::Flow event;
        event.name = expectName;
        database.InsertFlow(event);
    }
    std::vector<FlowPO> flowPOS;
    flowTable.Select(FlowColumn::NAME).ExcuteQuery(dbPtr, flowPOS);
    EXPECT_EQ(flowPOS.size(), CACHE_SIZE);
    EXPECT_EQ(flowPOS[0].name, expectName);
}

/**
 * text场景插入flow999条数据，数据库里有0条
 */
TEST_F(TextTraceDatabaseMockTest, TestInsertSliceWhenInsert999FlowThenDbHave0count)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    database.SetDbPtr(dbPtr);
    for (int i = 0; i < CACHE_SIZE - 1; ++i) {
        Trace::Flow event;
        event.name = "hhhhhhhh";
        database.InsertFlow(event);
    }
    std::vector<FlowPO> flowPOS;
    flowTable.Select(FlowColumn::NAME).ExcuteQuery(dbPtr, flowPOS);
    EXPECT_EQ(flowPOS.size(), 0);
}

/**
 * text场景插入counter1000条数据，数据库里有1000条
 */
TEST_F(TextTraceDatabaseMockTest, TestInsertSliceWhenInsert1000counterThenDbHave1000count)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    database.SetDbPtr(dbPtr);
    const std::string expectName = "hhhhh";
    for (int i = 0; i < CACHE_SIZE; ++i) {
        Trace::Counter event;
        event.name = expectName;
        database.InsertCounter(event);
    }
    std::vector<CounterPO> counterPOS;
    counterTable.Select(CounterColumn::NAME).ExcuteQuery(dbPtr, counterPOS);
    EXPECT_EQ(counterPOS.size(), CACHE_SIZE);
    EXPECT_EQ(counterPOS[0].name, expectName);
}

/**
 * text场景插入counter999条数据，数据库里有0条
 */
TEST_F(TextTraceDatabaseMockTest, TestInsertSliceWhenInsert999counterThenDbHave0count)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    database.SetDbPtr(dbPtr);
    const std::string expectName = "hhhhh";
    for (int i = 0; i < CACHE_SIZE - 1; ++i) {
        Trace::Counter event;
        event.name = expectName;
        database.InsertCounter(event);
    }
    std::vector<CounterPO> counterPOS;
    counterTable.Select(CounterColumn::NAME).ExcuteQuery(dbPtr, counterPOS);
    EXPECT_EQ(counterPOS.size(), 0);
}

/**
 * text场景修改线程名，如果未初始化返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateThreadNameWhenNotInitThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    std::string threadDataSql = "INSERT INTO \"main\".\"thread\" (\"track_id\", \"tid\", \"pid\", \"thread_name\", "
        "\"thread_sort_index\") VALUES (4, '15', '207552992', 'Plane 3', 15);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, threadDataSql);
    Trace::MetaData event;
    bool success = database.UpdateThreadName(event);
    EXPECT_EQ(success, false);
}
/**
 * text场景修改线程名，如果初始化了，但表不存在，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateThreadNameWhenTableNotExistThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    database.InitStmt();
    const std::string alterThreadTable = "DROP TABLE thread;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    Trace::MetaData event;
    bool success = database.UpdateThreadName(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改线程名，如果初始化了，表也存在啊，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateThreadNameWhenNormalThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    std::string threadDataSql = "INSERT INTO \"main\".\"thread\" (\"track_id\", \"tid\", \"pid\", \"thread_name\", "
        "\"thread_sort_index\") VALUES (4, '15', '207552992', 'Plane 3', 15);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, threadDataSql);
    database.InitStmt();
    std::vector<ThreadPO> threadPOS;
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    const uint64_t expectCount = 1;
    const std::string oldThreadName = "Plane 3";
    const std::string nowThreadName = "Plane 4";
    const std::string newTid = "17";
    const std::string newPid = "999";
    EXPECT_EQ(threadPOS.size(), expectCount);
    EXPECT_EQ(threadPOS[0].threadName, oldThreadName);
    Trace::MetaData event;
    event.trackId = threadPOS[0].trackId;
    event.args.name = nowThreadName;
    event.tid = newTid;
    event.pid = newPid;
    bool success = database.UpdateThreadName(event);
    EXPECT_EQ(success, true);
    threadPOS.clear();
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    EXPECT_EQ(threadPOS[0].threadName, nowThreadName);
    EXPECT_EQ(threadPOS[0].tid, newTid);
    EXPECT_EQ(threadPOS[0].pid, newPid);
}

/**
 * text场景修改线程顺序，如果未初始化返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateThreadSortIndexWhenNotInitThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    database.SetDbPtr(dbPtr);
    Trace::MetaData event;
    bool success = database.UpdateThreadSortIndex(event);
    EXPECT_EQ(success, false);
}
/**
 * text场景修改线程顺序，如果初始化了，但表不存在，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateThreadSortIndexWhenTableNotExistThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    database.InitStmt();
    const std::string alterThreadTable = "DROP TABLE thread;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    Trace::MetaData event;
    bool success = database.UpdateThreadSortIndex(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改线程名，如果初始化了，表也存在啊，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateThreadSortIndexWhenNormalThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    std::string threadDataSql = "INSERT INTO \"main\".\"thread\" (\"track_id\", \"tid\", \"pid\", \"thread_name\", "
        "\"thread_sort_index\") VALUES (4, '15', '207552992', 'Plane 3', 15);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, threadDataSql);
    database.InitStmt();
    std::vector<ThreadPO> threadPOS;
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    const uint64_t expectCount = 1;
    const uint64_t oldSort = 15;
    const uint64_t newSort = 19;
    EXPECT_EQ(threadPOS.size(), expectCount);
    EXPECT_EQ(threadPOS[0].threadSortIndex, oldSort);
    Trace::MetaData event;
    event.trackId = threadPOS[0].trackId;
    event.args.sortIndex = newSort;
    bool success = database.UpdateThreadSortIndex(event);
    EXPECT_EQ(success, true);
    threadPOS.clear();
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    EXPECT_EQ(threadPOS[0].threadSortIndex, newSort);
}

/**
 * text场景修改进程名，如果未初始化返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessNameWhenNotInitThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    std::string processDataSql = "INSERT INTO \"main\".\"process\" (\"pid\", \"process_name\", \"label\", "
        "\"process_sort_index\") VALUES ('20', 'SCALARLDST', 'kkk', 20);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, processDataSql);
    Trace::MetaData event;
    bool success = database.UpdateProcessName(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改进程名，如果初始化了，但表不存在，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessNameWhenTableNotExistThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    database.InitStmt();
    const std::string alterThreadTable = "DROP TABLE process;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    Trace::MetaData event;
    bool success = database.UpdateProcessName(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改进程名，如果初始化了，表也存在啊，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessNameWhenNormalThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    std::string processDataSql = "INSERT INTO \"main\".\"process\" (\"pid\", \"process_name\", \"label\", "
        "\"process_sort_index\") VALUES ('20', 'SCALARLDST', 'kkk', 20);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, processDataSql);
    database.InitStmt();
    std::vector<ProcessPO> processPOS;
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    const uint64_t expectCount = 1;
    const std::string oldProcessName = "SCALARLDST";
    const std::string nowProcessName = "kkkkkk";
    const std::string newPid = "20";
    EXPECT_EQ(processPOS.size(), expectCount);
    EXPECT_EQ(processPOS[0].processName, oldProcessName);
    Trace::MetaData event;
    event.args.name = nowProcessName;
    event.pid = newPid;
    bool success = database.UpdateProcessName(event);
    EXPECT_EQ(success, true);
    processPOS.clear();
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    EXPECT_EQ(processPOS[0].processName, nowProcessName);
}

/**
 * text场景修改进程label，如果未初始化返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessLabelWhenNotInitThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    std::string processDataSql = "INSERT INTO \"main\".\"process\" (\"pid\", \"process_name\", \"label\", "
        "\"process_sort_index\") VALUES ('20', 'SCALARLDST', 'kkk', 20);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, processDataSql);
    Trace::MetaData event;
    bool success = database.UpdateProcessLabel(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改进程label，如果初始化了，但表不存在，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessLabelWhenTableNotExistThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    database.InitStmt();
    const std::string alterThreadTable = "DROP TABLE process;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    Trace::MetaData event;
    bool success = database.UpdateProcessLabel(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改进程label，如果初始化了，表也存在啊，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessLabelWhenNormalThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    std::string processDataSql = "INSERT INTO \"main\".\"process\" (\"pid\", \"process_name\", \"label\", "
        "\"process_sort_index\") VALUES ('20', 'SCALARLDST', 'kkk', 20);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, processDataSql);
    database.InitStmt();
    std::vector<ProcessPO> processPOS;
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    const uint64_t expectCount = 1;
    const std::string oldProcessLable = "kkk";
    const std::string nowProcessLable = "mmmm";
    const std::string newPid = "20";
    EXPECT_EQ(processPOS.size(), expectCount);
    EXPECT_EQ(processPOS[0].label, oldProcessLable);
    Trace::MetaData event;
    event.args.labels = nowProcessLable;
    event.pid = newPid;
    bool success = database.UpdateProcessLabel(event);
    EXPECT_EQ(success, true);
    processPOS.clear();
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    EXPECT_EQ(processPOS[0].label, nowProcessLable);
}

/**
 * text场景修改进程顺序，如果未初始化返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessSortWhenNotInitThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    std::string processDataSql = "INSERT INTO \"main\".\"process\" (\"pid\", \"process_name\", \"label\", "
        "\"process_sort_index\") VALUES ('20', 'SCALARLDST', 'kkk', 20);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, processDataSql);
    Trace::MetaData event;
    bool success = database.UpdateProcessSortIndex(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改进程label，如果初始化了，但表不存在，返回false
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessSortWhenTableNotExistThenReturnFalse)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    database.InitStmt();
    const std::string alterThreadTable = "DROP TABLE process;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    Trace::MetaData event;
    bool success = database.UpdateProcessSortIndex(event);
    EXPECT_EQ(success, false);
}

/**
 * text场景修改进程label，如果初始化了，表也存在啊，返回true
 */
TEST_F(TextTraceDatabaseMockTest, TestUpdateProcessSortWhenNormalThenReturnTrue)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    std::string processDataSql = "INSERT INTO \"main\".\"process\" (\"pid\", \"process_name\", \"label\", "
        "\"process_sort_index\") VALUES ('20', 'SCALARLDST', 'kkk', 20);";
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::InsertData(dbPtr, processDataSql);
    database.InitStmt();
    std::vector<ProcessPO> processPOS;
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    const uint64_t expectCount = 1;
    const uint64_t oldProcessSort = 20;
    const uint64_t nowProcessSort = 30;
    const std::string newPid = "20";
    EXPECT_EQ(processPOS.size(), expectCount);
    EXPECT_EQ(processPOS[0].processSortIndex, oldProcessSort);
    Trace::MetaData event;
    event.args.sortIndex = nowProcessSort;
    event.pid = newPid;
    bool success = database.UpdateProcessSortIndex(event);
    EXPECT_EQ(success, true);
    processPOS.clear();
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    EXPECT_EQ(processPOS[0].processSortIndex, nowProcessSort);
}

/**
 * 算子调优场景测试CommitData，未初始化，插入失败
 */
TEST_F(TextTraceDatabaseMockTest, TestSimulationCommitDataWhenNotInitThenInsertFailed)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    const uint64_t trackId = 1;
    Trace::ThreadEvent threadEvent;
    threadEvent.trackId = trackId;
    threadEvent.tid = "ggg";
    threadEvent.pid = "lll";
    threadEvent.threadName = "mmmm";
    threadEvent.SetThreadSortIndex();
    database.AddSimulationThreadCache(threadEvent);
    database.CommitData();
    std::vector<ThreadPO> threadPOS;
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    EXPECT_EQ(threadPOS.size(), 0);
}

/**
 * 算子调优场景测试CommitData，初始化了，插入成功
 */
TEST_F(TextTraceDatabaseMockTest, TestSimulationCommitDataWhenInitThenInsertSuccess)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    const uint64_t trackId = 1;
    Trace::ThreadEvent threadEvent;
    threadEvent.trackId = trackId;
    threadEvent.tid = "ggg";
    threadEvent.pid = "lll";
    threadEvent.threadName = "mmmm";
    threadEvent.SetThreadSortIndex();
    database.InitStmt();
    database.AddSimulationThreadCache(threadEvent);
    database.CommitData();
    std::vector<ThreadPO> threadPOS;
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    const uint64_t expectSize = 1;
    EXPECT_EQ(threadPOS.size(), expectSize);
    EXPECT_EQ(threadPOS[0].trackId, trackId);
    EXPECT_EQ(threadPOS[0].tid, "ggg");
    EXPECT_EQ(threadPOS[0].pid, "lll");
    EXPECT_EQ(threadPOS[0].threadName, "mmmm");
    EXPECT_EQ(threadPOS[0].threadSortIndex, 0);
}

/**
 * 算子调优场景测试CommitData，初始化了，但表不存在，插入失败
 */
TEST_F(TextTraceDatabaseMockTest, TestSimulationCommitDataWhenInitAndTableNotExistThenInsertFailed)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, threadTableSql);
    const uint64_t trackId = 1;
    Trace::ThreadEvent threadEvent;
    threadEvent.trackId = trackId;
    threadEvent.tid = "ggg";
    threadEvent.pid = "lll";
    threadEvent.threadName = "mmmm";
    threadEvent.SetThreadSortIndex();
    database.InitStmt();
    database.AddSimulationThreadCache(threadEvent);
    const std::string alterThreadTable = "DROP TABLE thread;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    database.CommitData();
    std::vector<ThreadPO> threadPOS;
    threadTable.Select(ThreadColumn::TRACK_ID, ThreadColumn::TID)
        .Select(ThreadColumn::THREAD_NAME, ThreadColumn::THREAD_SORT_INDEX)
        .Select(ThreadColumn::PID)
        .ExcuteQuery(dbPtr, threadPOS);
    const uint64_t expectSize = 0;
    EXPECT_EQ(threadPOS.size(), expectSize);
}

/**
 * 算子调优场景进程数据测试CommitData，未初始化，插入失败
 */
TEST_F(TextTraceDatabaseMockTest, TestSimulationCommitDataWhenProcessNotInitThenInsertFailed)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    Trace::ProcessEvent processEvent;
    processEvent.pid = "yy";
    processEvent.processName = "mm";
    database.AddSimulationProcessCache(processEvent);
    database.CommitData();
    std::vector<ProcessPO> processPOS;
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    EXPECT_EQ(processPOS.size(), 0);
}

/**
 * 算子调优场景测试进程数据CommitData，初始化了，插入成功
 */
TEST_F(TextTraceDatabaseMockTest, TestSimulationCommitDataWhenProcessInitThenInsertSuccess)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    Trace::ProcessEvent processEvent;
    processEvent.pid = "yy";
    processEvent.processName = "mm";
    database.InitStmt();
    database.AddSimulationProcessCache(processEvent);
    database.CommitData();
    std::vector<ProcessPO> processPOS;
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    const uint64_t expectSize = 1;
    EXPECT_EQ(processPOS.size(), expectSize);
    EXPECT_EQ(processPOS[0].processName, "mm");
    EXPECT_EQ(processPOS[0].pid, "yy");
}

/**
 * 算子调优场景测试进程数据CommitData，初始化了，但表不存在，插入失败
 */
TEST_F(TextTraceDatabaseMockTest, TestSimulationCommitDataWhenProcessInitAndTableNotExistThenInsertFailed)
{
    std::recursive_mutex sqlMutex;
    MockDatabase database(sqlMutex);
    sqlite3 *dbPtr = nullptr;
    DatabaseTestCaseMockUtil::OpenDB(dbPtr);
    database.SetDbPtr(dbPtr);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, sliceTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, flowTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, counterTableSql);
    DatabaseTestCaseMockUtil::CreateTable(dbPtr, processSql);
    Trace::ProcessEvent processEvent;
    processEvent.pid = "yy";
    processEvent.processName = "mm";
    database.InitStmt();
    const std::string alterThreadTable = "DROP TABLE process;";
    DatabaseTestCaseMockUtil::ExecuteSql(dbPtr, alterThreadTable);
    database.AddSimulationProcessCache(processEvent);
    database.CommitData();
    std::vector<ProcessPO> processPOS;
    processTable.Select(ProcessColumn::PID, ProcessColumn::PROCESS_NAME)
        .Select(ProcessColumn::LABEL, ProcessColumn::PROCESS_SORT_INDEX)
        .ExcuteQuery(dbPtr, processPOS);
    const uint64_t expectSize = 0;
    EXPECT_EQ(processPOS.size(), expectSize);
}