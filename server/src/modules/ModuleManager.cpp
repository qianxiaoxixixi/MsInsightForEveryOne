/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2022. All rights reserved.
 */
#include "pch.h"
#include "GlobalModule.h"
#include "TimelineModule.h"
#include "SummaryModule.h"
#include "MemoryModule.h"
#include "CommunicationModule.h"
#include "OperatorModule.h"
#include "SourceModule.h"
#include "AdvisorModule.h"
#include "JupyterModule.h"
#include "WsSessionManager.h"
#include "ModuleManager.h"

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
    std::unique_ptr<SummaryModule> summaryModule = std::make_unique<SummaryModule>();
    std::unique_ptr<MemoryModule> memoryModule = std::make_unique<MemoryModule>();
    std::unique_ptr<CommunicationModule> communicationModule = std::make_unique<CommunicationModule>();
    std::unique_ptr<OperatorModule> operatorModule = std::make_unique<OperatorModule>();
    std::unique_ptr<SourceModule> sourceModule = std::make_unique<SourceModule>();
    std::unique_ptr<AdvisorModule> advisorModule = std::make_unique<AdvisorModule>();
    std::unique_ptr<JupyterModule> jupyterModule = std::make_unique<JupyterModule>();
    global->RegisterRequestHandlers();
    timelineModule->RegisterRequestHandlers();
    memoryModule->RegisterRequestHandlers();
    summaryModule->RegisterRequestHandlers();
    communicationModule->RegisterRequestHandlers();
    operatorModule->RegisterRequestHandlers();
    sourceModule->RegisterRequestHandlers();
    advisorModule->RegisterRequestHandlers();
    jupyterModule->RegisterRequestHandlers();
    moduleMap.emplace(MODULE_GLOBAL, std::move(global));
    moduleMap.emplace(MODULE_TIMELINE, std::move(timelineModule));
    moduleMap.emplace(MODULE_SUMMARY, std::move(summaryModule));
    moduleMap.emplace(MODULE_MEMORY, std::move(memoryModule));
    moduleMap.emplace(MODULE_COMMUNICATION, std::move(communicationModule));
    moduleMap.emplace(MODULE_OPERATOR, std::move(operatorModule));
    moduleMap.emplace(MODULE_SOURCE, std::move(sourceModule));
    moduleMap.emplace(MODULE_ADVISOR, std::move(advisorModule));
    moduleMap.emplace(MODULE_JUPYTER, std::move(jupyterModule));
}

void ModuleManager::UnRegister()
{
    std::unique_lock<std::mutex> lock(mutex);
    moduleMap.clear();
}

void ModuleManager::OnDispatchModuleRequest(std::unique_ptr<Request> request)
{
    auto moduleName = request->moduleName;
    if (moduleMap.count(moduleName) == 0) {
        ServerLog::Error("Failed to dispatch to module");
        return;
    }
    if (!WsSessionManager::Instance().CheckSession()) {
        ServerLog::Error("Invalid session found when dispatch");
        return;
    }
    ServerLog::Info("Dispatch to module, module = ", moduleName,
                    ", command = ", request->command, ", request id = ", request->id);
    moduleMap.at(moduleName)->OnRequest(std::move(request));
}
} // end of namespace Module
} // end of namespace Dic