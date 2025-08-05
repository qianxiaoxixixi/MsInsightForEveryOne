/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_RLMSTXCONFIGMANAGER_H
#define PROFILER_SERVER_RLMSTXCONFIGMANAGER_H
#include <vector>
#include "RLDomainObject.h"
namespace Dic::Module::RL {
class RLMstxConfigManager {
public:
    static RLMstxConfigManager &Instance();
    RLMstxConfigManager(const RLMstxConfigManager &) = delete;
    RLMstxConfigManager &operator=(const RLMstxConfigManager &) = delete;
    RLMstxConfigManager(RLMstxConfigManager &&) = delete;
    RLMstxConfigManager &operator=(RLMstxConfigManager &&) = delete;

    std::vector<RLMstxConfig> GetRLMstxConfig();
    std::vector<std::string> GetMstxTaskNameList();
    std::string GetTaskTypeByName(const std::string &name);
    RLMstxConfig GetMstxConfigByTaskName(const std::string &name);
private:
    std::vector<RLMstxConfig> config = {{"verl", "megatron",
                                         {{"ActorRollout", "generate_sequences", {}},
                                          {"Reward", "compute_log_prob", {}},
                                          {"Actor", "update_actor", {}},
                                          {"Reference", "compute_ref_log_prob", {}}}}};
    explicit RLMstxConfigManager() = default;
    ~RLMstxConfigManager() = default;
};
}
#endif // PROFILER_SERVER_RLMSTXCONFIGMANAGER_H
