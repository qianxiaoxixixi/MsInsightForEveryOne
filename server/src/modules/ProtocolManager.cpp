/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */
#include "pch.h"
#include "PluginsManager.h"
#include "ProtocolManager.h"

namespace Dic {
namespace Protocol {
using namespace Dic::Server;
ProtocolManager::ProtocolManager()
{
    Register();
}

ProtocolManager::~ProtocolManager()
{
    UnRegister();
}

void ProtocolManager::Register()
{
    std::unique_lock<std::mutex> lock(mutex);
    protocolMap.clear();
    auto& manager = Core::PluginsManager::Instance();
    for (const auto &[moduleName, plugin]: manager.GetAllPlugins()) {
        auto protocol = plugin->GetProtocolUtil();
        if (protocol != nullptr) {
            protocol->Register();
            protocolMap.emplace(moduleName, std::move(protocol));
        }
    }
}

void ProtocolManager::UnRegister()
{
    std::unique_lock<std::mutex> lock(mutex);
    protocolMap.clear();
}

std::unique_ptr<Request> ProtocolManager::FromJson(const std::string &requestStr, std::string &error)
{
    auto requestJson = JsonUtil::TryParse(requestStr, error);
    if (!requestJson.has_value()) {
        ServerLog::Warn("Failed to parse request json. ", requestStr);
        return nullptr;
    }
    if (!JsonUtil::IsJsonKeyValid(requestJson.value(), "moduleName")) {
        ServerLog::Warn(R"(Json Key "moduleName" is invalid, request=)", requestStr);
        return nullptr;
    }
    std::string moduleName = JsonUtil::GetString(requestJson.value(), "moduleName");
    if (moduleName.empty()) {
        ServerLog::Warn("Unknown module name, request=", requestStr);
        return nullptr;
    }
    std::unique_lock<std::mutex> lock(mutex);
    if (protocolMap.count(moduleName) == 0) {
        ServerLog::Warn("Failed to get request module protocol. module type:", moduleName);
        return nullptr;
    }
    return protocolMap.at(moduleName)->FromJson(requestJson.value(), error);
}

std::optional<document_t> ProtocolManager::ToJson(const Response &response, std::string &error)
{
    auto moduleName = response.moduleName;
    std::unique_lock<std::mutex> lock(mutex);
    if (protocolMap.count(moduleName) == 0) {
        ServerLog::Warn("Failed to get response module protocol. module type:", moduleName);
        return std::nullopt;
    }
    return protocolMap.at(moduleName)->ToJson(response, error);
}

std::optional<document_t> ProtocolManager::ToJson(const Event &event, std::string &error)
{
    auto moduleName = event.moduleName;
    std::unique_lock<std::mutex> lock(mutex);
    if (protocolMap.count(moduleName) == 0) {
        ServerLog::Warn("Failed to get event module protocol. module type:", moduleName);
        return std::nullopt;
    }
    return protocolMap.at(moduleName)->ToJson(event, error);
}
}
}