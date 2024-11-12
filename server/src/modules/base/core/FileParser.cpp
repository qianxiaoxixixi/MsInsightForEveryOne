/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */
#include "FileParser.h"

namespace Dic {
namespace Module {
std::string FileParser::GetError()
{
    return error;
}

void FileParser::SetParseEndCallBack(std::function<void(const std::string, bool result, const std::string)> &callback)
{
    paserEndCallback = callback;
}

void FileParser::SetParseProgressCallBack(
    std::function<void(const std::string, uint64_t parsedSize, uint64_t totalSize, int progress)> &callback)
{
    paserProgressCallback = callback;
}

void FileParser::Reset()
{
    error.clear();
    paserEndCallback = nullptr;
    paserProgressCallback = nullptr;
    fileProgressMap.clear();
}
} // end of namespace Module
} // end of namespace Dic