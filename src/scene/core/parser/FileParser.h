/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#ifndef DATA_INSIGHT_CORE_SCENE_CORE_FILE_PARSER_H
#define DATA_INSIGHT_CORE_SCENE_CORE_FILE_PARSER_H

#include <string>
#include <functional>

namespace Dic {
namespace Scene {
namespace Core {
class FileParser {
public:
    FileParser() = default;
    virtual ~FileParser() = default;
    virtual bool Parse(const std::string &filePath, const std::string &fileId) = 0;
    virtual bool WaitParseEnd() = 0;
    virtual void SetParseEndCallBack(std::function<void(const std::string)> &callback);
    virtual std::string GetError();

protected:
    std::string error;
    std::function<void(const std::string)> paserEndCallback;
};
} // end of namespace Core
} // end of namespace Scene
} // end of namespace Dic

#endif // DATA_INSIGHT_CORE_SCENE_CORE_FILE_PARSER_H