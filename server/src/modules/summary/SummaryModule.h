//
//  * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
//

#ifndef PROFILER_SERVER_SUMMARY_MODULE_H
#define PROFILER_SERVER_SUMMARY_MODULE_H

#include "BaseModule.h"

namespace Dic {
namespace Module {
class SummaryModule : public BaseModule {
public:
    SummaryModule();
    ~SummaryModule() override;

    void RegisterRequestHandlers() override;
    void OnRequest(std::unique_ptr<Protocol::Request> request) override;
};
} // end of namespace Module
} // end of namespace Dic
#endif // PROFILER_SERVER_SUMMARY_MODULE_H
