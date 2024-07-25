/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: Protocol Response declaration
 */

#ifndef DIC_GLOBAL_PROTOCOL_RESPONSE_H
#define DIC_GLOBAL_PROTOCOL_RESPONSE_H

#include <vector>
#include "GlobalDefs.h"
#include "ProtocolDefs.h"
#include "ProtocolMessage.h"

namespace Dic {
namespace Protocol {

// token.heartCheck
struct TokenHeartCheckResponse : public Response {
    TokenHeartCheckResponse() : Response(REQ_RES_HEART_CHECK) {};
};

struct File {
    File() = default;
    ~File() = default;
    File(const std::string &n, const std::string &p) : name(n), path(p) {}
    std::string name;
    std::string path;
};

struct Folder {
    std::string name;
    std::string path;
    std::vector<std::unique_ptr<Folder>> childrenFolders;
    std::vector<std::unique_ptr<File>> childrenFiles;
};

struct FilesGetResBody {
    std::string path;
    std::vector<std::unique_ptr<Folder>> childrenFolders;
    std::vector<std::unique_ptr<File>> childrenFiles;
    bool exist = false;
};

struct FilesGetResponse : public Response {
    FilesGetResponse() : Response(REQ_RES_FILES_GET) {};
    FilesGetResBody body;
};

struct ProjectExplorerInfoUpdateBody {
};

struct ProjectExplorerInfoUpdateResponse : public Response {
    ProjectExplorerInfoUpdateResponse() : Response(REQ_RES_PROJECT_EXPLORER_UPDATE) {};
    ProjectExplorerInfoUpdateBody body;
};

struct ProjectDirectoryInfo {
    std::string projectName;
    std::vector<std::string> fileName;
};

struct ProjectExplorerInfoGetBody {
    std::vector<ProjectDirectoryInfo> projectDirectoryList;
};

struct ProjectExplorerInfoGetResponse : public Response {
    ProjectExplorerInfoGetResponse() : Response(REQ_RES_PROJECT_EXPLORER_INFO_GET) {};
    ProjectExplorerInfoGetBody body;
};

struct ProjectExplorerInfoDeleteBody {
};

struct ProjectExplorerInfoDeleteResponse : public Response {
    ProjectExplorerInfoDeleteResponse() : Response(REQ_RES_PROJECT_EXPLORER_INFO_DELETE) {};
    ProjectExplorerInfoDeleteBody body;
};

struct ProjectConflictCheckBody {
    bool isConflict;
};

struct ProjectConflictCheckResponse : public Response {
    ProjectConflictCheckResponse() : Response(REQ_RES_PROJECT_CONFLICT_CHECK) {};
    ProjectConflictCheckBody body;
};
} // end of namespace Protocol
} // end of namespace Dic

#endif // DIC_GLOBAL_PROTOCOL_RESPONSE_H
