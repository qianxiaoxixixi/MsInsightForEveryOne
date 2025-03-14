/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_CLUSTERDOMAINOBJECT_H
#define PROFILER_SERVER_CLUSTERDOMAINOBJECT_H

#include <string>
#include <vector>
#include "CommunicationProtocolResponse.h"

namespace Dic {
namespace Module {
    struct MatrixInfoDo {
        int srcRank;
        int dstRank;
        std::string transportType;
        std::string opName;
        double transitSize = 0;
        double transitTime = 0;
        double bandwidth = 0;
    };

    struct OperatorTimeDo {
        std::string rankId;
        std::string operatorName;
        uint64_t startTime = 0;
        uint64_t elapseTime = 0;
    };

    struct DurationDo {
        std::string rankId;
        double startTime;
        double elapseTime = 0;
        double transitTime = 0;
        double synchronizationTime = 0;
        double waitTime = 0;
        double idleTime = 0;
        double synchronizationTimeRatio = 0;
        double waitTimeRatio = 0;
        double sdmaBw = 0;
        double rdmaBw = 0;
        double sdmaTime = 0;
        double rdmaTime = 0;
    };
}
}
#endif // PROFILER_SERVER_CLUSTERDOMAINOBJECT_H
