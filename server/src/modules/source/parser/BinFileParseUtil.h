/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#ifndef BINFILEPARSEUTIL_H
#define BINFILEPARSEUTIL_H

#include "SourceProtocol.h"

namespace Dic::Module::Source {
class BinFileParseUtil {
public:
    static std::string GetContentStr(std::ifstream& file, const Position& position, uint64_t maxSize = MAX_DATA_SIZE);
    static bool IsDataSizeExceedUpperLimit(uint64_t realSize, uint64_t upperLimit);
    static std::string GetSingleContentStrByDataType(std::ifstream &file, DataTypeEnum dataTypeEnum,
        std::map<int, std::vector<Position>> &curBlockMap);
private:
    static constexpr uint64_t MAX_DATA_SIZE = 1024 * 1024 * 100; // 100MB
};
}

#endif // BINFILEPARSEUTIL_H
