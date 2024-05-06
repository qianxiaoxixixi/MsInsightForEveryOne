/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 *
 */
#include "ServerLog.h"
#include "JsonUtil.h"
#include "SourceProtocolUtil.h"
#include "SourceProtocolRequest.h"
#include "SourceProtocolResponse.h"
#include "SourceProtocol.h"

namespace Dic {
namespace Protocol {
void SourceProtocol::RegisterJsonToRequestFuncs()
{
    jsonToReqFactory.emplace(REQ_RES_SOURCE_CODE_FILE, ToCodeFileRequest);
    jsonToReqFactory.emplace(REQ_RES_SOURCE_API_LINE, ToApiLineRequest);
    jsonToReqFactory.emplace(REQ_RES_SOURCE_API_INSTRUCTIONS, ToApiInstrRequest);
    jsonToReqFactory.emplace(REQ_RES_DETAILS_BASE_INFO, ToDetailsBaseInfoRequest);
    jsonToReqFactory.emplace(REQ_RES_DETAILS_COMPUTE_LOAD_INFO, ToDetailsLoadInfoRequest);
}

void SourceProtocol::RegisterResponseToJsonFuncs()
{
    resToJsonFactory.emplace(REQ_RES_SOURCE_CODE_FILE, ToCodeFileResponse);
    resToJsonFactory.emplace(REQ_RES_SOURCE_API_LINE, ToApiLineResponse);
    resToJsonFactory.emplace(REQ_RES_SOURCE_API_INSTRUCTIONS, ToApiInstrResponse);
    resToJsonFactory.emplace(REQ_RES_DETAILS_BASE_INFO, ToDetailsBaseInfoResponse);
    resToJsonFactory.emplace(REQ_RES_DETAILS_COMPUTE_LOAD_INFO, ToDetailsLoadInfoResponse);
}

void SourceProtocol::RegisterEventToJsonFuncs()
{
}

#pragma region <<Json To Request>>

std::unique_ptr<Request> SourceProtocol::ToCodeFileRequest(const Dic::json_t &json, std::string &error)
{
    std::unique_ptr<SourceCodeFileRequest> reqPtr = std::make_unique<SourceCodeFileRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.sourceName, json["params"], "sourceName");
    return reqPtr;
}

std::unique_ptr<Request> SourceProtocol::ToApiLineRequest(const Dic::json_t &json, std::string &error)
{
    std::unique_ptr<SourceApiLineRequest> reqPtr = std::make_unique<SourceApiLineRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.coreName, json["params"], "coreName");
    JsonUtil::SetByJsonKeyValue(reqPtr->params.sourceName, json["params"], "sourceName");
    return reqPtr;
}

std::unique_ptr<Request> SourceProtocol::ToApiInstrRequest(const Dic::json_t &json, std::string &error)
{
    std::unique_ptr<SourceApiInstrRequest> reqPtr = std::make_unique<SourceApiInstrRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    return reqPtr;
}

std::unique_ptr<Request> SourceProtocol::ToDetailsBaseInfoRequest(const Dic::json_t &json, std::string &error)
{
    std::unique_ptr<SourceDetailBaseInfoRequest> reqPtr = std::make_unique<SourceDetailBaseInfoRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    return reqPtr;
}

std::unique_ptr<Request> SourceProtocol::ToDetailsLoadInfoRequest(const Dic::json_t &json, std::string &error)
{
    std::unique_ptr<SourceDetailsLoadInfoRequest> reqPtr = std::make_unique<SourceDetailsLoadInfoRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    return reqPtr;
}

#pragma endregion

#pragma region <<Reponse To Json>>

std::optional<document_t> SourceProtocol::ToCodeFileResponse(const Dic::Protocol::Response &response)
{
    return ToResponseJson<SourceCodeFileResponse>(dynamic_cast<const SourceCodeFileResponse &>(response));
}

std::optional<document_t> SourceProtocol::ToApiLineResponse(const Dic::Protocol::Response &response)
{
    return ToResponseJson<SourceApiLineResponse>(dynamic_cast<const SourceApiLineResponse &>(response));
}

std::optional<document_t> SourceProtocol::ToApiInstrResponse(const Dic::Protocol::Response &response)
{
    return ToResponseJson<SourceApiInstrResponse>(dynamic_cast<const SourceApiInstrResponse &>(response));
}

std::optional<document_t> SourceProtocol::ToDetailsBaseInfoResponse(const Dic::Protocol::Response &response)
{
    return ToResponseJson<DetailsBaseInfoResponse>(dynamic_cast<const DetailsBaseInfoResponse &>(response));
}

std::optional<document_t> SourceProtocol::ToDetailsLoadInfoResponse(const Dic::Protocol::Response &response)
{
    return ToResponseJson<DetailsLoadInfoResponse>(dynamic_cast<const DetailsLoadInfoResponse &>(response));
}
#pragma endregion

    } // namespace Protocol
} // namespace Dic