/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 */

#include "ServerLog.h"
#include "GlobalModule.h"
#include "ModuleManager.h"
#include "TimelineModule.h"

namespace Dic {
namespace Module {
using namespace Dic::Server;
ModuleManager &ModuleManager::Instance()
{
    static ModuleManager instance;
    return instance;
}

ModuleManager::ModuleManager()
{
    Register();
}

ModuleManager::~ModuleManager()
{
    UnRegister();
}

void ModuleManager::Register()
{
    std::unique_lock<std::mutex> lock(mutex);
    moduleMap.clear();
    std::unique_ptr<GlobalModule> global = std::make_unique<GlobalModule>();
    std::unique_ptr<TimelineModule> timelineModule = std::make_unique<TimelineModule>();
    global->RegisterRequestHandlers();
    timelineModule->RegisterRequestHandlers();
    moduleMap.emplace(ModuleType::GLOBAL, std::move(global));
    moduleMap.emplace(ModuleType::TIMELINE, std::move(timelineModule));
}

void ModuleManager::UnRegister()
{
    std::unique_lock<std::mutex> lock(mutex);
    moduleMap.clear();
}

bool ModuleManager::SetGlobalConfig(const GlobalConfig &config)
{
    if (moduleMap.count(ModuleType::GLOBAL) == 0) {
        return false;
    }
    ((GlobalModule &)(*moduleMap.at(ModuleType::GLOBAL).get())).Config(config);
    return true;
}

bool ModuleManager::SetTimelineConfig(const TimelineConfig &config) {
    if (moduleMap.count(ModuleType::TIMELINE) == 0) {
        return false;
    }
    ((TimelineModule &)(*moduleMap.at(ModuleType::TIMELINE).get())).Config(config);
    return true;
}

const std::optional<GlobalConfig> ModuleManager::GetGlobalConfig()
{
    if (moduleMap.count(ModuleType::GLOBAL) == 0) {
        return std::nullopt;
    }
    return ((GlobalModule &)(*moduleMap.at(ModuleType::GLOBAL).get())).GetConfig();
}

const std::optional<TimelineConfig> ModuleManager::GetTimelineConfig() {
    if (moduleMap.count(ModuleType::TIMELINE) == 0) {
        return std::nullopt;
    }
    return ((TimelineModule &)(*moduleMap.at(ModuleType::TIMELINE).get())).GetConfig();
}

void ModuleManager::OnDispatchModuleRequest(std::unique_ptr<Request> request)
{
    if (moduleMap.count(request->scene) == 0) {
        ServerLog::Error("Failed to dispatch to scene, scene = ", ENUM_TO_STR(request->scene).value(),
            ", token = ", StringUtil::AnonymousString(request->token), ", command = ", request->command);
        return;
    }
    int callbackId = request->resultCallbackId.has_value() ? request->resultCallbackId.value() : -1;
    ServerLog::Info("Dispatch to scene, scene = ", ENUM_TO_STR(request->scene).value(),
        ", token = ", StringUtil::AnonymousString(request->token), ", command = ", request->command,
        ", request id = ", request->id, ", callback id = ", callbackId);
    moduleMap.at(request->scene)->OnRequest(std::move(request));
}
} // end of namespace Module
} // end of namespace Dic