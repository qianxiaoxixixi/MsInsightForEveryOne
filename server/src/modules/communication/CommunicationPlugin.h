// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef PROFILER_SERVER_COMMUNICATIONPLUGIN_H
#define PROFILER_SERVER_COMMUNICATIONPLUGIN_H
#include "BasePlugin.h"
#include "ProtocolDefs.h"
#include "CommunicationModule.h"
#include "CommunicationProtocol.h"
namespace Dic::Module::Communication {
class CommunicationPlugin : public Core::BasePlugin {
public:
    CommunicationPlugin() : Core::BasePlugin(MODULE_COMMUNICATION) {};
    std::unique_ptr<Module::BaseModule> GetModule() override
    {
        return std::make_unique<CommunicationModule>();
    }
    std::unique_ptr<Module::ProtocolUtil> GetProtocolUtil() override
    {
        return std::make_unique<CommunicationProtocol>();
    }
};
}
#endif // PROFILER_SERVER_COMMUNICATIONPLUGIN_H
