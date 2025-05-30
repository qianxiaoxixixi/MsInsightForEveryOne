/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import type { NotificationInterceptor } from './defs';
import { type DataSource } from '@/centralServer/websocket/defs';
import { updateSession } from '@/connection/notificationHandler';
import { store } from '@/store';
import { runInAction } from 'mobx';

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

interface ParseStatisticNotification {
    rankIds: string[];
}

interface ParseHeatmapNotification {
    parseResult: boolean;
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

export const parseStatisticSuccessHandler: NotificationInterceptor<ParseStatisticNotification> = (data): void => {
    const session = store.sessionStore.activeSession;
    const iERankIds: Set<string> = new Set([...session.iERankIds]);
    data.rankIds.forEach((item) => {
        if (!iERankIds.has(item)) {
            iERankIds.add(item);
        }
    });
    updateSession({ iERankIds: [...iERankIds] });
};

export const profilingExpertDataParsedHandler: NotificationInterceptor<ParseHeatmapNotification> = (data): void => {
    const session = store.sessionStore.activeSession;

    runInAction(() => {
        session.profilingExpertDataParsed = data.parseResult ?? false;
    });
};
