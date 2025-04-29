/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_EXPERTHOTSPOTMANAGER_H
#define PROFILER_SERVER_EXPERTHOTSPOTMANAGER_H
#include <string>
#include "ClusterDef.h"
#include "VirtualClusterDatabase.h"
namespace Dic {
namespace Module {
namespace Summary {
class ExpertHotspotManager {
public:
    static bool InitExpertHotspotData(const std::string &filePath, const std::string &version, std::string &errorMsg);
    static std::vector<ExpertHotspotStruct> QueryExpertHotspotData(const std::string &modelStage,
                                                                    const std::string &version);
private:
    static bool FillExpertIdInfo(std::vector<ExpertHotspotStruct> &hotspotInfos, const ModelInfo &modelInfo,
                                 const std::vector<ExpertDeploymentStruct> &deployment);
    static bool SaveModelInfo(const ModelInfo &modelInfo, std::shared_ptr<VirtualClusterDatabase> &db);
    static ModelInfo GetModelInfo(std::shared_ptr<VirtualClusterDatabase> &db);
};
}
}
}
#endif // PROFILER_SERVER_EXPERTHOTSPOTMANAGER_H
