/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "MetaDataCacheManager.h"

namespace Dic::Module::Timeline {
void MetaDataCacheManager::Clear()
{
    std::unique_lock<std::shared_mutex> uniqueLock(sharedMutex);
    hcclGroupInfoMap.clear();
    distributedArgsInfo.reset();
}

void MetaDataCacheManager::AddParallelGroupInfo(const std::vector<ParallelGroupInfo>& parallelGroupInfoList)
{
    std::unique_lock<std::shared_mutex> uniqueLock(sharedMutex);
    for (const auto &item: parallelGroupInfoList) {
        if (item.group.empty()) {
            continue;
        }
        hcclGroupInfoMap[item.group] = item;
    }
}

std::optional<ParallelGroupInfo> MetaDataCacheManager::GetParallelGroupInfo(const std::string &group)
{
    std::shared_lock<std::shared_mutex> sharedLock(sharedMutex);
    auto res = hcclGroupInfoMap.find(group);
    if (res != hcclGroupInfoMap.end()) {
        return {res->second};
    }
    return {};
}

void MetaDataCacheManager::SetDistributedArgsInfo(const std::optional<DistributedArgs> &args)
{
    std::unique_lock<std::shared_mutex> uniqueLock(sharedMutex);
    distributedArgsInfo = args;
}

std::optional<DistributedArgs> MetaDataCacheManager::GetDistributedArgsInfo()
{
    std::shared_lock<std::shared_mutex> sharedLock(sharedMutex);
    return distributedArgsInfo;
}
}