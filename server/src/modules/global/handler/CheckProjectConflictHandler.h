/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_CHECKPROJECTCONFLICTHANDLER_H
#define PROFILER_SERVER_CHECKPROJECTCONFLICTHANDLER_H

#include "GlobalHandler.h"
#include "GlobalProtocolRequest.h"

namespace Dic {
namespace Module {
class CheckProjectConflictHandler : public GlobalHandler {
public:
    CheckProjectConflictHandler()
    {
        command = REQ_RES_PROJECT_CONFLICT_CHECK;
    }
    ~CheckProjectConflictHandler() override = default;

    void HandleRequest(std::unique_ptr<Request> requestPtr) override;
};
} // end of namespace Module
} // Dic

#endif // PROFILER_SERVER_CHECKPROJECTCONFLICTHANDLER_H
