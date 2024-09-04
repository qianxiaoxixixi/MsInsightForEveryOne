// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.

#ifndef PROFILER_SERVER_ROOFLINEPARSER_H
#define PROFILER_SERVER_ROOFLINEPARSER_H

#include <memory>
#include "RooflineParserImpl.h"
#include "SourceProtocolUtil.h"

namespace Dic::Module::Source {
class RooflineParser {
public:
    RooflineParser() : impl(std::make_unique<RooflineParserImpl>())
    {};

    bool GetDetailsRoofline(const std::string &jsonStr, Protocol::DetailsRooflineBody &responseBody)
    {
        if (impl != nullptr) {
            return impl->GetDetailsRoofline(jsonStr, responseBody);
        }
        return false;
    }

private:
    std::unique_ptr<RooflineParserImpl> impl;
};
}


#endif // PROFILER_SERVER_ROOFLINEPARSER_H
