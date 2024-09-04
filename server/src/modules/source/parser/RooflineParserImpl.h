// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.

#ifndef PROFILER_SERVER_ROOFLINEPARSERIMPL_H
#define PROFILER_SERVER_ROOFLINEPARSERIMPL_H
#include "SourceProtocolUtil.h"
namespace Dic::Module::Source {
class RooflineParserImpl {
public:
    bool GetDetailsRoofline(const std::string &jsonStr, Protocol::DetailsRooflineBody &responseBody);

    static Protocol::SubBlockUnitData ParseSubBlockUnitData(const json_t &item);

    static Protocol::RooflineGraph ParseRooflineData(const json_t &item);

    static Protocol::Roofline ParseRoofline(const json_t &item);

    static std::optional<Protocol::RooflineData> ConvertStrToRooflineData(const std::string &json);
};
}


#endif // PROFILER_SERVER_ROOFLINEPARSERIMPL_H
