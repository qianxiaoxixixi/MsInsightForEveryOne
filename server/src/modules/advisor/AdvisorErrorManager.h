/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_ADVISORERRORMANAGER_H
#define PROFILER_SERVER_ADVISORERRORMANAGER_H

#include <map>
#include <string>
#include "ModuleRequestHandler.h"

namespace Dic::Module::Advisor {
// 错误码枚举
enum class ErrorCode {
    RESET_ERROR = 0,
    // 接口入参错误
    PARAMS_ERROR = 1101,

    // 中间变量
    GET_DEVICE_ID_FAILED = 2001,
    GET_ABSOLUTE_PATH_FAILED = 2101,

    // 数据库错误
    CONNECT_DATABASE_FAILED = 3001,
    QUERY_ACLNN_OPERATOR_COUNT_FAILED = 3101,
    QUERY_AFFINITY_API_FAILED = 3102,
    DATA_ANOMALY_END_TIME_SMALLER_TIMESTAMP = 3103,
    QUERY_AFFINITY_OPTIMIZER_FAILED = 3104,
    QUERY_AI_CPU_OP_CAN_BE_OPTIMIZED_FAILED = 3105,
    QUERY_OPERATOR_DISPATCH_FAILED = 3106,
};

// 错误信息映射
extern const std::map<ErrorCode, std::string> errorMessages;

const std::string& GetErrorMessage(ErrorCode code);
void SetAdvisorError(ErrorCode code);
}  // namespace Dic::Module::Advisor

#endif  // PROFILER_SERVER_ADVISORERRORMANAGER_H
