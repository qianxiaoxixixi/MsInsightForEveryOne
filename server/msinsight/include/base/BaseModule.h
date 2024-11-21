// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef MSINSIGHT_BASE_MODULE_H
#define MSINSIGHT_BASE_MODULE_H
#include "ProtocolUtil.h"
#include "ModuleRequestHandler.h"
namespace Dic::Module {
class BaseModule {
public:
    BaseModule() = default;
    virtual ~BaseModule() = default;
    virtual void RegisterRequestHandlers() = 0;
    virtual void OnRequest(std::unique_ptr<Protocol::Request> request);
protected:
    std::string moduleName = MODULE_UNKNOWN;
    std::map<std::string, std::unique_ptr<ModuleRequestHandler>> requestHandlerMap;
};
} // end of namespace Module
#endif // MSINSIGHT_BASE_MODULE_H
