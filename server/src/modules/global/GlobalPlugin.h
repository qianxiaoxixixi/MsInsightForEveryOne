// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_GLOBALPLUGIN_H
#define PROFILER_SERVER_GLOBALPLUGIN_H
#include "BasePlugin.h"
#include "GlobalModule.h"
#include "GlobalProtocol.h"
namespace Dic::Module::Global {
class GlobalPlugin : public Core::BasePlugin {
public:
    GlobalPlugin() : Core::BasePlugin(MODULE_GLOBAL) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<GlobalModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<GlobalProtocol>();
    }
};
}
#endif // PROFILER_SERVER_GLOBALPLUGIN_H
