// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_SUMMARYPLUGIN_H
#define PROFILER_SERVER_SUMMARYPLUGIN_H
#include "BasePlugin.h"
#include "SummaryModule.h"
#include "SummaryProtocol.h"
namespace Dic::Module::Summary {
class SummaryPlugin : public Core::BasePlugin {
public:
    SummaryPlugin() : Core::BasePlugin(MODULE_SUMMARY) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<SummaryModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<SummaryProtocol>();
    }
};
}
#endif // PROFILER_SERVER_SummaryPLUGIN_H
