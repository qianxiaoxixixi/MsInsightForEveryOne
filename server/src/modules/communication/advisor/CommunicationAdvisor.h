/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_COMMUNICATION_ADVISOR_H
#define PROFILER_SERVER_COMMUNICATION_ADVISOR_H

#include "CommunicationProtocolResponse.h"

namespace Dic {
namespace Module {
namespace Communication {
using namespace Protocol;

class CommunicationAdvisor {
public:
    CommunicationAdvisor() = default;
    ~CommunicationAdvisor() = default;
    static void GenerateAdvisor(std::vector<CommunicationAdvisorInfo> &items);
};
} // end of namespace Communication
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_COMMUNICATION_ADVISOR_H
