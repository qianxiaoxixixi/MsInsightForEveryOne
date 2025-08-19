/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_QUERYTABLEDATADETAILHANDLER_H
#define PROFILER_SERVER_QUERYTABLEDATADETAILHANDLER_H
#include "VirtualTraceDatabase.h"
#include "TimelineRequestHandler.h"
namespace Dic::Module::Timeline {
class QueryTableDataDetailHandler : public TimelineRequestHandler {
public:
    QueryTableDataDetailHandler()
    {
        command = Protocol::REQ_RES_TABLE_DATA_DETAIL;
    }
    ~QueryTableDataDetailHandler() override = default;
    bool HandleRequest(std::unique_ptr<Dic::Protocol::Request> requestPtr) override;
    static void ComputeTableDetail(const TableDataDetailRequest &request, TableDataDetailResponse &response,
                                   std::shared_ptr<VirtualTraceDatabase> databasePtr) ;

    void ComputeLinkPageDetail(TableDataDetailRequest &request, TableDataDetailResponse &response,
                               const std::shared_ptr<VirtualTraceDatabase> &database) const;
};
}  // namespace Dic::Module::IE
#endif  // PROFILER_SERVER_QUERYTABLEDATADETAILHANDLER_H
