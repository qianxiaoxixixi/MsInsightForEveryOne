/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef PROFILER_SERVER_IMPORTACTIONANALYZER_H
#define PROFILER_SERVER_IMPORTACTIONANALYZER_H
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include "TimelineProtocolResponse.h"

namespace Dic::Module::Timeline {

class ImportActionAnalyzer {
public:
    explicit ImportActionAnalyzer();
    ~ImportActionAnalyzer() = default;
};
}
#endif // PROFILER_SERVER_IMPORTACTIONANALYZER_H
