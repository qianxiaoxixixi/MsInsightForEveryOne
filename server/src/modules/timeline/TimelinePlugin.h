// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_TIMELINEPLUGIN_H
#define PROFILER_SERVER_TIMELINEPLUGIN_H
#include "BasePlugin.h"
#include "TimelineModule.h"
#include "TimelineProtocol.h"
namespace Dic::Module::Timeline {
class TimelinePlugin : public Core::BasePlugin {
public:
    TimelinePlugin() : Core::BasePlugin(MODULE_TIMELINE) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<TimelineModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<TimelineProtocol>();
    }
};
}
#endif // PROFILER_SERVER_TIMELINEPLUGIN_H
