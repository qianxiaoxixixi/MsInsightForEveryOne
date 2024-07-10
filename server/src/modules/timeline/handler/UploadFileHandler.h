/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_UPLOADFILEHANDLER_H
#define PROFILER_SERVER_UPLOADFILEHANDLER_H
#include "TimelineRequestHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
class UploadFileHandler : public TimelineRequestHandler {
public:
    UploadFileHandler()
    {
        command = REQ_RES_UPLOAD_FILE;
    }
    ~UploadFileHandler() override = default;
    void HandleRequest(std::unique_ptr<Request> requestPtr) override;
};
} // Timeline
} // Module
} // Dic
#endif // PROFILER_SERVER_UPLOADFILEHANDLER_H
