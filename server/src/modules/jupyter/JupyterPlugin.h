// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_JUPYTERPLUGIN_H
#define PROFILER_SERVER_JUPYTERPLUGIN_H
#include "BasePlugin.h"
#include "JupyterModule.h"
namespace Dic::Module::Jupyter {
class JupyterPlugin : public Core::BasePlugin {
public:
    JupyterPlugin() : Core::BasePlugin(MODULE_JUPYTER) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<JupyterModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return nullptr;
    }
};
}
#endif // PROFILER_SERVER_JUPYTERPLUGIN_H
