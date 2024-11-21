// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_SOURCEPLUGIN_H
#define PROFILER_SERVER_SOURCEPLUGIN_H
#include "BasePlugin.h"
#include "SourceModule.h"
#include "SourceProtocol.h"
namespace Dic::Module::Source {
class SourcePlugin : public Core::BasePlugin {
public:
    SourcePlugin() : Core::BasePlugin(MODULE_SOURCE) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<SourceModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<SourceProtocol>();
    }
};
}
#endif // PROFILER_SERVER_SOURCEPLUGIN_H
