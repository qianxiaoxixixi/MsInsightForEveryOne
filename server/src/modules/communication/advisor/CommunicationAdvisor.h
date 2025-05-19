/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_COMMUNICATION_ADVISOR_H
#define PROFILER_SERVER_COMMUNICATION_ADVISOR_H

#include "CommunicationBaseAnalyzer.h"

namespace Dic {
namespace Module {
namespace Communication {
using namespace Protocol;

class CommunicationAdvisor {
public:
    CommunicationAdvisor() = default;
    ~CommunicationAdvisor() = default;
    void GenerateAdvisor(std::vector<CommunicationAdvisorInfo> &items);
    void Register();
protected:
    std::map<std::string, std::unique_ptr<CommunicationBaseAnalyzer>> advisorMap;
};
} // end of namespace Communication
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_COMMUNICATION_ADVISOR_H
