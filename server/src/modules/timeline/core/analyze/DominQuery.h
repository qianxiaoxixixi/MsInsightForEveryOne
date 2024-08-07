/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#ifndef PROFILER_SERVER_SLICEQUERY_H
#define PROFILER_SERVER_SLICEQUERY_H
#include <string>
#include "sqlite3.h"
namespace Dic::Module::Timeline {
struct SliceQuery {
    sqlite3 *db;
    uint64_t trackId = 0;
    /* *
     * 界面选取的开始时间
     */
    uint64_t startTime = 0;
    /* *
     * 界面选取的结束时间
     */
    uint64_t endTime = 0;
    /* *
     * 泳道在数据库中的最小时间
     */
    uint64_t minTimestamp = 0;
    std::string cat;
    /*
     * timeline框选时使用
    */
    std::string rankId;
    std::string pid;
    std::string tid;
    std::string metaType;
    bool isFilterPythonFunction = false;
    bool QueryThreadsCheck(std::string &error) const
    {
        error.clear();
        if (db == nullptr) {
            error = "database connection is not open";
            return false;
        }
        if (startTime > endTime) {
            error = "start time is bigger than end time";
            return false;
        }
        if (trackId == 0) {
            error = "track id is not correct";
            return false;
        }
        return true;
    }
};

struct FlowQuery {
    sqlite3 *db = nullptr;
    uint64_t trackId = 0;
    /* *
     * 连线的id
     */
    std::string flowId;
    /* *
     * 界面选取的开始时间
     */
    uint64_t startTime = 0;
    /* *
     * 界面选取的结束时间
     */
    uint64_t endTime = 0;
    /* *
     * 泳道在数据库中的最小时间
     */
    uint64_t minTimestamp = 0;
};

struct ThreadQuery {
    sqlite3 *db = nullptr;
};
}
#endif // PROFILER_SERVER_SLICEQUERY_H
