/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_METADATACACHEMANAGER_H
#define PROFILER_SERVER_METADATACACHEMANAGER_H
#include <map>
#include <string>
#include <optional>
#include <shared_mutex>
#include "DomainObject.h"
#include "ClusterDef.h"

namespace Dic::Module::Timeline {
class MetaDataCacheManager {
public:
    static MetaDataCacheManager &Instance()
    {
        static MetaDataCacheManager metaDataCacheManager;
        return metaDataCacheManager;
    }

    void Clear();
    void AddParallelGroupInfo(const std::vector<ParallelGroupInfo>& parallelGroupInfoList);
    std::optional<ParallelGroupInfo> GetParallelGroupInfo(const std::string& group);
    void SetDistributedArgsInfo(const std::optional<DistributedArgs> &args);
    std::optional<DistributedArgs> GetDistributedArgsInfo();
private:
    MetaDataCacheManager() = default;
    ~MetaDataCacheManager() = default;
    std::map<std::string, ParallelGroupInfo> hcclGroupInfoMap;
    std::optional<DistributedArgs> distributedArgsInfo;
    std::shared_mutex sharedMutex;
};
}
#endif // PROFILER_SERVER_METADATACACHEMANAGER_H
