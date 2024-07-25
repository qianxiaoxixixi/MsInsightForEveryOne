/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "WsSessionManager.h"
#include "UploadFileParser.h"
#include "UploadFileHandler.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Dic::Module::Timeline;
using namespace Dic::Server;
void UploadFileHandler::HandleRequest(std::unique_ptr<Request> requestPtr)
{
    UploadFileRequest &request = dynamic_cast<UploadFileRequest &>(*requestPtr.get());
    UploadFileParser::Instance().Parse(request);
}
} // Timeline
} // Module
} // Dic