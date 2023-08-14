/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#ifndef DATA_INSIGHT_CORE_SCENE_CORE_EVENT_PARSER_H
#define DATA_INSIGHT_CORE_SCENE_CORE_EVENT_PARSER_H

#include <string>

namespace Dic {
namespace Scene {
namespace Core {
class EventParser {
public:
    EventParser(const std::string &filePath, const std::string &dbPath);
    ~EventParser() = default;
    void Parse(uint64_t startPosition, uint64_t endPosition);

private:
    std::string filePath;
    std::string dbPath;
    int parseCount = 0;
    int ignoreCount = 0;
};
} // end of namespace Core
} // end of namespace Scene
} // end of namespace Dic

#endif // DATA_INSIGHT_CORE_SCENE_CORE_EVENT_PARSER_H
