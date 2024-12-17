/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_SERVER_SRC_MODULES_SUMMARY_HANDLER_QUERYPARALLELISMARRANGEMENTHANDLER_H
#define PROFILER_SERVER_SERVER_SRC_MODULES_SUMMARY_HANDLER_QUERYPARALLELISMARRANGEMENTHANDLER_H

#include "SummaryProtocolResponse.h"
#include "SummaryRequestHandler.h"
namespace Dic::Module::Summary {
class QueryParallelismArrangementHandler : public SummaryRequestHandler  {
public:
    QueryParallelismArrangementHandler()
    {
        command = Protocol::REQ_RES_PARALLELISM_ARRANGEMENT_ALL;
    }
    ~QueryParallelismArrangementHandler() override = default;
    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
}
#endif // PROFILER_SERVER_SERVER_SRC_MODULES_SUMMARY_HANDLER_QUERYPARALLELISMARRANGEMENTHANDLER_H
