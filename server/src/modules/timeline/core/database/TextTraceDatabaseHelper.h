/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_TEXTTRACEDATABASEHELPER_H
#define PROFILER_SERVER_TEXTTRACEDATABASEHELPER_H

#include "ClusterDef.h"
#include "TimelineProtocolResponse.h"
#include "DomainObject.h"
#include "FlowAnalyzer.h"

namespace Dic::Module::Timeline {
class TextTraceDatabaseHelper {
public:
    static void ProcessByteAlignmentAnalyzerDataForText(std::vector<CommunicationLargeOperatorInfo> &result,
        std::vector<std::pair<std::string, std::string>> rawData);
    static void AssembleUnitFlowsBody(Protocol::UnitFlowsBody &responseBody, uint64_t minTimestamp,
        std::unordered_map<std::string, std::vector<FlowPoint>> &flowPointMap) ;
};
}

#endif // PROFILER_SERVER_TEXTTRACEDATABASEHELPER_H
