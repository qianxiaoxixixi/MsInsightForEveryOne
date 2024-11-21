// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_OPERATORPLUGIN_H
#define PROFILER_SERVER_OPERATORPLUGIN_H
#include "BasePlugin.h"
#include "OperatorModule.h"
#include "OperatorProtocol.h"
namespace Dic::Module::Operator {
class OperatorPlugin : public Core::BasePlugin {
public:
    OperatorPlugin() : Core::BasePlugin(MODULE_OPERATOR) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<OperatorModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<OperatorProtocol>();
    }
};
}
#endif // PROFILER_SERVER_OPERATORPLUGIN_H
