/*
* Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/

#ifndef PROFILER_SERVER_MOCKDATABASE_H
#define PROFILER_SERVER_MOCKDATABASE_H
#include <iostream>
#include <sqlite3.h>
#include <string>
#include "Database.h"
#include "DbTraceDataBase.h"
#include "TextTraceDatabase.h"
namespace Dic::Mock {
    class MockDatabase {
    public:
        static std::shared_ptr<Dic::Module::Database> GetMockDatabase()
        {
            class MockDatabase : public Dic::Module::Database {
            public:
                explicit MockDatabase(std::recursive_mutex& sqlMutex) : Dic::Module::Database(sqlMutex)
                {
                }
                void SetDbPtr(sqlite3* dbPtr)
                {
                    isOpen = true;
                    db = dbPtr;
                    path = ":memory:";
                }
            };
            std::recursive_mutex sqlMutex;
            std::shared_ptr<MockDatabase> ptr = std::make_shared<MockDatabase>(sqlMutex);
            sqlite3* dbPtr = nullptr;
            OpenDB(dbPtr);
            ptr->SetDbPtr(dbPtr);
            return ptr;
        }

        static std::shared_ptr<Module::FullDb::DbTraceDataBase> GetMockDbTraceDatabase()
        {
            class MockDatabase : public Module::FullDb::DbTraceDataBase {
            public:
                explicit MockDatabase(std::recursive_mutex& sqlMutex) : Module::FullDb::DbTraceDataBase(sqlMutex)
                {
                }
                void SetDbPtr(sqlite3* dbPtr)
                {
                    isOpen = true;
                    db = dbPtr;
                    path = ":memory:";
                    InitStringsCache();
                }
            };
            std::recursive_mutex sqlMutex;
            std::shared_ptr<MockDatabase> ptr = std::make_shared<MockDatabase>(sqlMutex);
            sqlite3* dbPtr = nullptr;
            OpenDB(dbPtr);
            ptr->SetDbPtr(dbPtr);
            return ptr;
        }

        static std::shared_ptr<Dic::Module::Timeline::TextTraceDatabase> GetMockTextTraceDatabase()
        {
            class MockDatabase : public Dic::Module::Timeline::TextTraceDatabase {
            public:
                explicit MockDatabase(std::recursive_mutex& sqlMutex) : Dic::Module::Timeline::TextTraceDatabase(sqlMutex)
                {
                }
                void SetDbPtr(sqlite3* dbPtr)
                {
                    isOpen = true;
                    db = dbPtr;
                    path = ":memory:";
                }
            };
            std::recursive_mutex sqlMutex;
            std::shared_ptr<MockDatabase> ptr = std::make_shared<MockDatabase>(sqlMutex);
            sqlite3* dbPtr = nullptr;
            OpenDB(dbPtr);
            ptr->SetDbPtr(dbPtr);
            return ptr;
        }

    private:
        static void OpenDB(sqlite3 *&db)
        {
            // 在内存中创建SQLite数据库
            int rc = sqlite3_open(":memory:", &db);
            if (rc != SQLITE_OK) {
                std::cout << "create db failed" << std::endl;
                sqlite3_close(db);
            }
        }
    };
}
#endif // PROFILER_SERVER_MOCKDATABASE_H
