/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_FILEMENUDATABASEDEF_H
#define PROFILER_SERVER_FILEMENUDATABASEDEF_H

#include <string>
#include <vector>

namespace Dic {
namespace Module {
namespace Global {
struct ProjectExplorerInfo {
    int64_t id;
    std::string projectName;
    std::string fileName;
    int64_t projectType;
    std::string importType;
    std::vector<std::string> dbPath;
};
}
}
}
#endif // PROFILER_SERVER_FILEMENUDATABASEDEF_H
