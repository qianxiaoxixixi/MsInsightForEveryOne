/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import { type ResponseInterceptor } from './defs';
import { useLoading } from '@/hooks/useLoading';
import { type DataSource, ProjectActionEnum } from '@/centralServer/websocket/defs';
import { useDataSources } from '@/stores/dataSource';
import { type UpdateProjectExplorerParam } from '@/stores/resourceComp';
import { useCompareConfig } from '@/stores/compareConfig';
import { console } from '@/utils/console';

interface ImportActionBody {
    subdirectoryList: string[];
    result: Array<{ rankId: string; dataPathList: string[] }>;
}

export interface ImportActionResponse {
    dataSource: DataSource;
    body: ImportActionBody;
}

export const importActionHandler: ResponseInterceptor<ImportActionResponse> = (event, data): void => {
    try {
        // 关闭遮罩
        useLoading().close();
        // 构造更新目录结构参数
        const dataSource = data.dataSource;
        const projectName = dataSource.projectName;
        const subdirectory = event.data.remote.dataPath as string[];
        const subdirectoryForUpdate = data.body.subdirectoryList;
        const isConflict = event.data.args.params.isConflict as boolean;
        const projectAction = event.data.args.params.projectAction as ProjectActionEnum;
        const param = { projectName, subdirectory, subdirectoryForUpdate, isConflict, projectAction } as UpdateProjectExplorerParam;
        const ranInfoList = data.body.result;
        // 先更新rank信息
        useCompareConfig().updateDataInfoMap(projectAction, projectName, ranInfoList);
        // 目录结构更新
        useDataSources().updateProjectExplorer(param);
    } catch (error) {
        console.error(error);
    }
};
