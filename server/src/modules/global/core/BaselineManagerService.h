// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.

#ifndef PROFILER_SERVER_BASELINEMANAGERSERVCE_H
#define PROFILER_SERVER_BASELINEMANAGERSERVCE_H
#include <string>
#include "GlobalDefs.h"
#include "GlobalProtocolRequest.h"
#include "SystemMemoryDatabaseDef.h"
namespace Dic {
namespace Module {
namespace Global {
class BaselineManagerService {
public:
    static void ResetBaseline();
    static bool InitBaselineData(const Protocol::BaselineSettingRequest &request, BaselineInfo &baselineInfo);
private:
    static bool IsClusterBaseline(ProjectTypeEnum projectTypeEnum,
                                  const std::vector<ProjectExplorerInfo> &projectInfoList,
                                  const std::string &filePath);
    static void InitBaselineParallelStrategy(const std::string &compareClusterPath);
    static bool CheckIsSupportCompare(const std::vector<ProjectExplorerInfo> &baseline,
                                      const std::vector<ProjectExplorerInfo> &cur,
                                      std::string &errorMsg, const std::string &filePath);
};
}
}
}
#endif // PROFILER_SERVER_BASELINEMANAGERSERVCE_H
