// Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
#ifndef DIC_PROTOCOL_ENUM_UTIL_H
#define DIC_PROTOCOL_ENUM_UTIL_H
#include "ProtocolUtil.h"
namespace Dic::Protocol {
const std::string REQUEST_NAME = "request";
const std::string RESPONSE_NAME = "response";
const std::string EVENT_NAME = "event";
enum class LinkType : int {
    SOCKET = 0,
    WEBSOCKET
};
template <typename ENUM> using EnumStrMap = std::map<ENUM, std::string>;
const EnumStrMap<ProtocolMessage::Type> PROTOCOL_MESSAGE_TYPE_ES = { { ProtocolMessage::Type::REQUEST, REQUEST_NAME },
                                                                     { ProtocolMessage::Type::RESPONSE, RESPONSE_NAME },
                                                                     { ProtocolMessage::Type::EVENT, EVENT_NAME } };
const EnumStrMap<Protocol::LinkType> LINK_TYPE_ES = { { Protocol::LinkType::WEBSOCKET, "websocket" },
                                                      { Protocol::LinkType::SOCKET, "socket" } };
template <typename E> std::optional<std::string> ENUM_TO_STR(const E &e)
{
    return std::nullopt;
}
template <typename E> std::optional<E> STR_TO_ENUM(const std::string &s)
{
    return std::nullopt;
}
template <typename E, typename M> std::optional<E> TryGetEnum(const M &map, const std::string &s)
{
    for (const auto &iter : map) {
        if (iter.second == s) {
            return iter.first;
        }
    }
    return std::nullopt;
}
template <> inline std::optional<std::string> ENUM_TO_STR<ProtocolMessage::Type>(const ProtocolMessage::Type &e)
{
    return PROTOCOL_MESSAGE_TYPE_ES.count(e) == 0 ? std::nullopt : std::make_optional<>(PROTOCOL_MESSAGE_TYPE_ES.at(e));
}
template <> inline std::optional<ProtocolMessage::Type> STR_TO_ENUM<ProtocolMessage::Type>(const std::string &s)
{
    return TryGetEnum<ProtocolMessage::Type>(PROTOCOL_MESSAGE_TYPE_ES, s);
}
template <> inline std::optional<std::string> ENUM_TO_STR<Protocol::LinkType>(const Protocol::LinkType &e)
{
    return LINK_TYPE_ES.count(e) == 0 ? std::nullopt : std::make_optional<>(LINK_TYPE_ES.at(e));
}
template <> inline std::optional<Protocol::LinkType> STR_TO_ENUM<Protocol::LinkType>(const std::string &s)
{
    return TryGetEnum<Protocol::LinkType>(LINK_TYPE_ES, s);
}
} // end of namespace Protocol
#endif // DIC_PROTOCOL_ENUM_UTIL_H
