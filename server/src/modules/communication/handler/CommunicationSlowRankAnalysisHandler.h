/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_COMMUNICATIONSLOWRANKANALYSISHANDLER_H
#define PROFILER_SERVER_COMMUNICATIONSLOWRANKANALYSISHANDLER_H

#include "ProtocolMessage.h"
#include "ProtocolDefs.h"
#include "CommunicationRequestHandler.h"

namespace Dic::Module::Communication {
class CommunicationSlowRankAnalysisHandler : public CommunicationRequestHandler {
public:
    CommunicationSlowRankAnalysisHandler()
    {
        command = Protocol::REQ_RES_COMMUNICATION_DURATION_SLOW_RANK_LIST;
    }
    ~CommunicationSlowRankAnalysisHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
}

#endif // PROFILER_SERVER_COMMUNICATIONSLOWRANKANALYSISHANDLER_H
