// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_ADVISORPLUGIN_H
#define PROFILER_SERVER_ADVISORPLUGIN_H
#include "BasePlugin.h"
#include "ProtocolDefs.h"
#include "AdvisorModule.h"
#include "AdvisorProtocolUtil.h"
namespace Dic::Module::Advisor {
class AdvisorPlugin : public Core::BasePlugin {
public:
    AdvisorPlugin() : Core::BasePlugin(MODULE_ADVISOR) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<AdvisorModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<AdvisorProtocolUtil>();
    }
};
}
#endif // PROFILER_SERVER_ADVISORPLUGIN_H
