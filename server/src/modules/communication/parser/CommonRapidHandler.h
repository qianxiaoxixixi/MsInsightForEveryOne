/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_COMMONRAPIDHANDLER_H
#define PROFILER_SERVER_COMMONRAPIDHANDLER_H

#include <unordered_map>
#include <string>
#include "ClusterDef.h"
#include "TextClusterDatabase.h"

namespace Dic {
namespace Module {
namespace Timeline {
    class CommonRapidHandler {
    protected:
        bool isOldData = false;
        std::unordered_map<std::string, CommGroupParallelInfo> groupIdsMap;
        uint64_t groupIdIndex = 0;
        std::shared_ptr<TextClusterDatabase> database;
        std::string GenerateAndGetGroupInfoId(const std::string &rankSet, const std::string &groupIdHash);
        void InitGroupInfoMap();
        bool SaveGroupInfoMap();
    };
}
}
}
#endif // PROFILER_SERVER_COMMONRAPIDHANDLER_H
