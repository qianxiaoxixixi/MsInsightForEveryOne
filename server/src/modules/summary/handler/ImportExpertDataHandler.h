/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#ifndef PROFILER_SERVER_IMPORTEXPERTHOTSHANDLER_H
#define PROFILER_SERVER_IMPORTEXPERTHOTSHANDLER_H

#include "SummaryRequestHandler.h"
namespace Dic {
namespace Module {
namespace Summary {
class ImportExpertDataHandler : public SummaryRequestHandler {
public:
    ImportExpertDataHandler()
    {
        command = Protocol::REQ_RES_IMPORT_EXPERT_DATA;
    }
    ~ImportExpertDataHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
}
}
}
#endif // PROFILER_SERVER_IMPORTEXPERTHOTSHANDLER_H
