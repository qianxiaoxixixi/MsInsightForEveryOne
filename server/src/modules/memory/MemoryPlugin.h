// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_MEMORYPLUGIN_H
#define PROFILER_SERVER_MEMORYPLUGIN_H
#include "BasePlugin.h"
#include "MemoryModule.h"
#include "MemoryProtocol.h"
namespace Dic::Module::Memory {
class MemoryPlugin : public Core::BasePlugin {
public:
    MemoryPlugin() : Core::BasePlugin(MODULE_MEMORY) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<MemoryModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<MemoryProtocol>();
    }
};
}
#endif // PROFILER_SERVER_MEMORYPLUGIN_H
