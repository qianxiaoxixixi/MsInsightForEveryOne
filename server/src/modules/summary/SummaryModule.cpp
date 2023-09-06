/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "SummaryModule.h"
#include "SummaryTopRankHandler.h"
#include "SummaryStatisticsHandler.h"

namespace Dic {
namespace Module {
using namespace Dic::Module::Summary;
SummaryModule::SummaryModule() : BaseModule()
{
    moduleName = ModuleType::SUMMARY;
}

SummaryModule::~SummaryModule()
{
    requestHandlerMap.clear();
}

void SummaryModule::RegisterRequestHandlers()
{
    requestHandlerMap.clear();
    requestHandlerMap.emplace(REQ_RES_SUMMARY_QUERY_TOP_DATA, std::make_unique<SummaryTopRankHandler>());
    requestHandlerMap.emplace(REQ_RES_SUMMARY_STATISTIC, std::make_unique<SummaryStatisticsHandler>());
}

void SummaryModule::OnRequest(std::unique_ptr<Protocol::Request> request)
{
    BaseModule::OnRequest(std::move(request));
};
} // end of namespace Module
} // end of namespace Dic