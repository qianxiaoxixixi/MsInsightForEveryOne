/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#ifndef PROFILER_SERVER_QUERYTABLEDATANAMELISTHANDLER_H
#define PROFILER_SERVER_QUERYTABLEDATANAMELISTHANDLER_H
#include "TimelineRequestHandler.h"
namespace Dic::Module::Timeline {
class QueryTableDataNameListHandler : public TimelineRequestHandler {
public:
    QueryTableDataNameListHandler()
    {
        command = Protocol::REQ_RES_TABLE_DATA_NAME_LIST;
    };

    ~QueryTableDataNameListHandler() override = default;

    bool HandleRequest(std::unique_ptr<Protocol::Request> requestPtr) override;
};
} // end of namespace Timeline
// end of namespace Module
// end of namespace Dic
#endif // PROFILER_SERVER_QUERYTABLEDATANAMELISTHANDLER_H
