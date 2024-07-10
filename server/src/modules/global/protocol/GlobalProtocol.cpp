/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2023. All rights reserved.
 */

#include "pch.h"
#include "GlobalProtocolRequest.h"
#include "GlobalProtocolResponse.h"
#include "GlobalProtocolUtil.h"
#include "GlobalProtocol.h"

namespace Dic {
namespace Protocol {
void GlobalProtocol::RegisterJsonToRequestFuncs()
{
    jsonToReqFactory.emplace(REQ_RES_TOKEN_CREATE, ToTokenCreateRequest);
    jsonToReqFactory.emplace(REQ_RES_TOKEN_DESTROY, ToTokenDestroyRequest);
    jsonToReqFactory.emplace(REQ_RES_TOKEN_CHECK, ToTokenCheckRequest);
    jsonToReqFactory.emplace(REQ_RES_TOKEN_HEART_CHECK, ToTokenHeartCheckRequest);
    jsonToReqFactory.emplace(REQ_RES_FILES_GET, ToFilesGetRequest);
    jsonToReqFactory.emplace(REQ_RES_PROJECT_EXPLORER_UPDATE, ToProjectExplorerUpdateRequest);
    jsonToReqFactory.emplace(REQ_RES_PROJECT_EXPLORER_INFO_GET, ToProjectExplorerInfoGetRequest);
    jsonToReqFactory.emplace(REQ_RES_PROJECT_EXPLORER_INFO_DELETE, ToProjectExplorerInfoDeleteRequest);
    jsonToReqFactory.emplace(REQ_RES_PROJECT_CONFLICT_CHECK, ToProjectConflictCheckRequest);
}

void GlobalProtocol::RegisterResponseToJsonFuncs()
{
    resToJsonFactory.emplace(REQ_RES_TOKEN_CREATE, ToTokenCreateResponseJson);
    resToJsonFactory.emplace(REQ_RES_TOKEN_DESTROY, ToTokenDestroyResponseJson);
    resToJsonFactory.emplace(REQ_RES_TOKEN_CHECK, ToTokenCheckResponseJson);
    resToJsonFactory.emplace(REQ_RES_TOKEN_HEART_CHECK, ToTokenHeartCheckResponseJson);
    resToJsonFactory.emplace(REQ_RES_FILES_GET, ToFilesGetResponseJson);
    resToJsonFactory.emplace(REQ_RES_PROJECT_EXPLORER_UPDATE, ToProjectExplorerInfoUpdateResponseJson);
    resToJsonFactory.emplace(REQ_RES_PROJECT_EXPLORER_INFO_GET, ToProjectExplorerInfoGetResponseJson);
    resToJsonFactory.emplace(REQ_RES_PROJECT_EXPLORER_INFO_DELETE, ToProjectExplorerInfoDeleteResponseJson);
    resToJsonFactory.emplace(REQ_RES_PROJECT_CONFLICT_CHECK, ToProjectConflictCheckResponseJson);
}

void GlobalProtocol::RegisterEventToJsonFuncs()
{
}

#pragma region <<Json To Request>>
std::unique_ptr<Request> GlobalProtocol::ToTokenCreateRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<TokenCreateRequest> reqPtr = std::make_unique<TokenCreateRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    if (JsonUtil::IsJsonKeyValid(json, "params")) {
        JsonUtil::SetByJsonKeyValue(reqPtr->params.deadTime, json["params"], "deadTime");
        JsonUtil::SetByJsonKeyValue(reqPtr->params.parentToken, json["params"], "parentToken");
    }
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToTokenDestroyRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<TokenDestroyRequest> reqPtr = std::make_unique<TokenDestroyRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.destroyToken, json["params"], "destroyToken");
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToTokenCheckRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<TokenCheckRequest> reqPtr = std::make_unique<TokenCheckRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.checkedToken, json["params"], "checkedToken");
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToTokenHeartCheckRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<TokenHeartCheckRequest> reqPtr = std::make_unique<TokenHeartCheckRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToFilesGetRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<FilesGetRequest> reqPtr = std::make_unique<FilesGetRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request base info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.path, json["params"], "path");
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToProjectExplorerUpdateRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<ProjectExplorerInfoUpdateRequest> reqPtr = std::make_unique<ProjectExplorerInfoUpdateRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request project explorer update info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.newProjectName, json["params"], "newProjectName");
    JsonUtil::SetByJsonKeyValue(reqPtr->params.oldProjectName, json["params"], "oldProjectName");
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToProjectExplorerInfoGetRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<ProjectExplorerInfoGetRequest> reqPtr = std::make_unique<ProjectExplorerInfoGetRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request project explorer get info, command is: " + reqPtr->command;
        return nullptr;
    }
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToProjectExplorerInfoDeleteRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<ProjectExplorerInfoDeleteRequest> reqPtr = std::make_unique<ProjectExplorerInfoDeleteRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request project explorer delete info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.projectName, json["params"], "projectName");
    if (json["params"].HasMember("dataPath") && json["params"]["dataPath"].IsArray()) {
        for (const auto &item : json["params"]["dataPath"].GetArray()) {
            reqPtr->params.dataPath.emplace_back(item.GetString());
        }
    }
    return reqPtr;
}

std::unique_ptr<Request> GlobalProtocol::ToProjectConflictCheckRequest(const json_t &json, std::string &error)
{
    std::unique_ptr<ProjectConflictCheckRequest> reqPtr = std::make_unique<ProjectConflictCheckRequest>();
    if (!ProtocolUtil::SetRequestBaseInfo(*reqPtr, json)) {
        error = "Failed to set request project explorer delete info, command is: " + reqPtr->command;
        return nullptr;
    }
    JsonUtil::SetByJsonKeyValue(reqPtr->params.projectName, json["params"], "projectName");
    if (json["params"].HasMember("dataPath") && json["params"]["dataPath"].IsArray()) {
        for (const auto &item : json["params"]["dataPath"].GetArray()) {
            reqPtr->params.dataPath.emplace_back(item.GetString());
        }
    }
    return reqPtr;
}

#pragma endregion

#pragma region <<Response To Json>>
std::optional<document_t> GlobalProtocol::ToTokenCreateResponseJson(const Response &response)
{
    return ToResponseJson<TokenCreateResponse>(dynamic_cast<const TokenCreateResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToTokenDestroyResponseJson(const Response &response)
{
    return ToResponseJson<TokenDestroyResponse>(dynamic_cast<const TokenDestroyResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToTokenCheckResponseJson(const Response &response)
{
    return ToResponseJson<TokenCheckResponse>(dynamic_cast<const TokenCheckResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToTokenHeartCheckResponseJson(const Response &response)
{
    return ToResponseJson<TokenHeartCheckResponse>(dynamic_cast<const TokenHeartCheckResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToFilesGetResponseJson(const Response &response)
{
    return ToResponseJson<FilesGetResponse>(dynamic_cast<const FilesGetResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToProjectExplorerInfoUpdateResponseJson(const Response &response)
{
    return ToResponseJson<ProjectExplorerInfoUpdateResponse>(
            dynamic_cast<const ProjectExplorerInfoUpdateResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToProjectExplorerInfoGetResponseJson(const Response &response)
{
    return ToResponseJson<ProjectExplorerInfoGetResponse>(
            dynamic_cast<const ProjectExplorerInfoGetResponse &>(response));
}
std::optional<document_t> GlobalProtocol::ToProjectExplorerInfoDeleteResponseJson(const Response &response)
{
    return ToResponseJson<ProjectExplorerInfoDeleteResponse>(
            dynamic_cast<const ProjectExplorerInfoDeleteResponse &>(response));
}

std::optional<document_t> GlobalProtocol::ToProjectConflictCheckResponseJson(const Response &response)
{
    return ToResponseJson<ProjectConflictCheckResponse>(
            dynamic_cast<const ProjectConflictCheckResponse &>(response));
}
#pragma endregion

#pragma region <<Event To Json>>
#pragma endregion
} // namespace Protocol
} // namespace Dic
