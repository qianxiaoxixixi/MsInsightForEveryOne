// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.

#ifndef PROFILER_SERVER_BASELINEMANAGERSERVCE_H
#define PROFILER_SERVER_BASELINEMANAGERSERVCE_H
#include <string>
#include "GlobalDefs.h"
#include "SystemMemoryDatabaseDef.h"
namespace Dic {
namespace Module {
namespace Global {
class BaselineManagerService {
public:
    static void ResetBaseline();
    static bool InitBaselineData(const std::string &projectName, const std::string &filePath,
        BaselineInfo &baselineInfo);
private:
    static bool IsClusterBaseline(ProjectTypeEnum projectTypeEnum, const std::string &fileName);
    static void InitBaselineParallelStrategy();
};
}
}
}
#endif // PROFILER_SERVER_BASELINEMANAGERSERVCE_H
