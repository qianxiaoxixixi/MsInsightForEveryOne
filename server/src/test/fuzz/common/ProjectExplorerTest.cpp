/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include <fstream>
#include "FuzzDefs.h"
#include "FuzzFileUtil.h"
#include "UpdateProjectExplorerInfoHandler.h"
#include "JsonUtil.h"

using namespace Dic::Module;

TEST(TestProjectExplorer, GetProjectExplorerInfoHandler) {
    char testApi[] = "test_get_project_explorer_info";
    DT_FUZZ_START(0, FUZZ_RUN_TIMES, testApi, 0)
        {
            auto request = std::make_unique<Dic::Module::ProjectExplorerInfoGetRequest>();
            Dic::Module::GetProjectExplorerInfoHandler handler;
            bool result = handler.HandleRequest(std::move(request));
            EXPECT_EQ(result, true);
        }
    DT_FUZZ_END()
}

TEST(TestProjectExplorer, UpdateProjectExplorerInfoHandler) {
    char testApi[] = "test_update_project_explorer_info";
    DT_FUZZ_START(0, FUZZ_RUN_TIMES, testApi, 0)
        {
            char* newProjectName = DT_SetGetString(&g_Element[0], 5, UINT32_MAX, "path");
            char* oldProjectName = DT_SetGetString(&g_Element[0], 5, UINT32_MAX, "path");
            auto requestPtr = std::make_unique<ProjectExplorerInfoUpdateRequest>();
            requestPtr->params.newProjectName = newProjectName;
            requestPtr->params.oldProjectName = oldProjectName;
            UpdateProjectExplorerInfoHandler updateProjectExplorerInfoHandler;
            updateProjectExplorerInfoHandler.HandleRequest(std::move(requestPtr));
        }
    DT_FUZZ_END()
}