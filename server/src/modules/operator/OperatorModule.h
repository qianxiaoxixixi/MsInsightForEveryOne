/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_OPERATORMODULE_H
#define PROFILER_SERVER_OPERATORMODULE_H

#include "BaseModule.h"

namespace Dic::Module {
    class OperatorModule : public BaseModule {
    public:
        OperatorModule();

        ~OperatorModule() override;

        void RegisterRequestHandlers() override;

        void OnRequest(std::unique_ptr<Protocol::Request> request) override;
    };
}

#endif // PROFILER_SERVER_OPERATORMODULE_H
