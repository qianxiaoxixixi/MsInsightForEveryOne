/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_ACLNNOPADVISOR_H
#define PROFILER_SERVER_ACLNNOPADVISOR_H

#include "AdvisorProtocolRequest.h"
#include "AdvisorProtocolResponse.h"
#include "VirtualTraceDatabase.h"

namespace Dic::Module::Advisor {
const uint64_t ACLNN_OP_CNT_THRESHOLD = 20;
const std::vector<std::string> SINGLE_OP_ORDER_BY_NAME_LIST = {
    "startTime", "duration", "pid", "tid", "name"
};
class AclnnOpAdvisor {
public:
    static bool Process(const Protocol::APITypeParams& params, Protocol::AclnnOperatorResBody& resBody);
private:
    static bool AclnnOpProcess(const std::shared_ptr<Timeline::VirtualTraceDatabase>& database,
        Protocol::KernelDetailsParams &param, Protocol::AclnnOperatorResBody& resBody);
};

} // Dic::Module::Advisor

#endif // PROFILER_SERVER_ACLNNOPADVISOR_H
