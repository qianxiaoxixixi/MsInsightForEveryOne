/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import type { NotificationInterceptor } from './defs';
import { type DataSource } from '../centralServer/websocket/defs';
import { updateSession } from '@/connection/notificationHandler';
import { store } from '@/store';

interface ImportActionBody {
    subdirectoryList: string[];
    result: Array<{ rankId: string; dataPathList: string[] }>;
    isBinary: boolean;
    isCluster: boolean;
    isIpynb: boolean;
    isPending: boolean;
    isSimulation: boolean;
    isOnlyTraceJson: boolean;
    reset: boolean;
}

export interface ImportActionResponse {
    dataSource: DataSource;
    body: ImportActionBody;
}

interface MemoryResult {
    hasMemory: boolean;
    rankId: string;
}
interface ParseMemoryNotification {
    memoryResult: MemoryResult[];
}

export const parseMemorySuccessHandler: NotificationInterceptor<ParseMemoryNotification> = (data): void => {
    const session = store.sessionStore.activeSession;
    const memoryRankIds: string[] = [...session.memoryRankIds];
    data.memoryResult.forEach((item) => {
        if (!memoryRankIds.includes(item.rankId) && item.hasMemory) {
            memoryRankIds.push(item.rankId);
        }
    });
    updateSession({ memoryRankIds });
};
