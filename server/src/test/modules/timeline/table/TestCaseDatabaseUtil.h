/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */


#ifndef PROFILER_SERVER_TESTCASEDATABASEUTIL_H
#define PROFILER_SERVER_TESTCASEDATABASEUTIL_H
#include <sqlite3.h>
#include <string>
namespace Dic::Protocol {
    using namespace Dic::Module::Timeline;
}
namespace Dic::TimeLine::TestCaseUtil {
class TestCaseDatabaseUtil {
public:
    static void CreateDatabse(sqlite3 *&db, const std::string &sql)
    {
        // 在内存中创建SQLite数据库
        int rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            return;
        }
        rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_free(nullptr);
            sqlite3_close(db);
            return;
        }
    }

    static void InsertData(sqlite3 *&db, const std::string &sql)
    {
        int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_free(nullptr);
            sqlite3_close(db);
            return;
        }
    }
};
}
#endif // PROFILER_SERVER_TESTCASEDATABASEUTIL_H
