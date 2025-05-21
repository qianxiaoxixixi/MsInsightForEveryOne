/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
#include "OperatorRequestHandler.h"
#include "QueryOpCategoryInfoHandler.h"
#include "QueryOpComputeUnitHandler.h"
#include "QueryOpStatisticInfoHandler.h"
#include "QueryOpDetailInfoHandler.h"
#include "QueryOpMoreInfoHandler.h"
#include "ExportOpDetailsHandler.h"
#include "ProtocolDefs.h"
#include "OperatorModule.h"

namespace Dic::Module {
    using namespace Operator;

    OperatorModule::OperatorModule()
    {
        moduleName = MODULE_OPERATOR;
    }

    OperatorModule::~OperatorModule()
    {
        requestHandlerMap.clear();
    }

    void OperatorModule::RegisterRequestHandlers()
    {
        requestHandlerMap.clear();
        requestHandlerMap.emplace(REQ_RES_OPERATOR_CATEGORY_INFO, std::make_unique<QueryOpCategoryInfoHandler>());
        requestHandlerMap.emplace(REQ_RES_OPERATOR_COMPUTE_UNIT_INFO, std::make_unique<QueryOpComputeUnitHandler>());
        requestHandlerMap.emplace(REQ_RES_OPERATOR_STATISTIC_INFO, std::make_unique<QueryOpStatisticInfoHandler>());
        requestHandlerMap.emplace(REQ_RES_OPERATOR_DETAIL_INFO, std::make_unique<QueryOpDetailInfoHandler>());
        requestHandlerMap.emplace(REQ_RES_OPERATOR_MORE_INFO, std::make_unique<QueryOpMoreInfoHandler>());
        requestHandlerMap.emplace(REQ_RES_OPERATOR_EXPORT_DETAILS, std::make_unique<ExportOpDetailsHandler>());
    }

    void OperatorModule::OnRequest(std::unique_ptr<Protocol::Request> request)
    {
        BaseModule::OnRequest(std::move(request));
    }

}
