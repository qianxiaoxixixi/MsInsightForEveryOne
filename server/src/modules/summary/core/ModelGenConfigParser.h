/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_MODELGENCONFIGPARSER_H
#define PROFILER_SERVER_MODELGENCONFIGPARSER_H
#include <vector>
#include "ClusterDef.h"
namespace Dic::Module::Summary {
class ModelGenConfigParser {
public:
    static ModelGenConfig ParserModelGenConfigByFilePath(const std::string &filePath, std::string &error);
};
}
#endif // PROFILER_SERVER_MODELGENCONFIGPARSER_H
