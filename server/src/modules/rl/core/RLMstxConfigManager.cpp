/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "RLMstxConfigManager.h"
#include <set>

namespace Dic::Module::RL {
    RLMstxConfigManager &RLMstxConfigManager::Instance()
    {
        static RLMstxConfigManager instance;
        return instance;
    }

    std::vector<RLMstxConfig> RLMstxConfigManager::GetRLMstxConfig()
    {
        return config;
    }

    std::vector<std::string> RLMstxConfigManager::GetMstxTaskNameList()
    {
        std::set<std::string> resSet;
        for (const auto &item: config) {
            for (const auto &task: item.taskConfigs) {
                resSet.insert(task.taskName);
            }
        }
        return std::vector<std::string>(resSet.begin(), resSet.end());
    }

    std::string RLMstxConfigManager::GetTaskTypeByName(const std::string &name)
    {
        std::string type = "";
        for (const auto &item: config) {
            for (const auto &task: item.taskConfigs) {
                if (name == task.taskName) {
                    return task.roleName;
                }
            }
        }
        return type;
    }

    RLMstxConfig RLMstxConfigManager::GetMstxConfigByTaskName(const std::string &name)
    {
        return {};
    }
}
