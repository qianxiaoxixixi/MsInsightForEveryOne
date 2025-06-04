/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import type { NotificationInterceptor } from './defs';
import { type DataSource } from '@/centralServer/websocket/defs';
import { updateSession } from '@/connection/notificationHandler';
import { store } from '@/store';
import type { CardInfo } from '@/entity/session';
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
    rankId: string; // 实际是 cardId, rankId 应该只有数字，而 cardId 可能在前面带有 host，形如: `{host} {rankId}`
    dbPath?: string;
    index: number;
}
interface ParseMemoryNotification {
    memoryResult: MemoryResult[];
}
interface ParseOperatorNotification {
    rankId: string; // 实际是 cardId, rankId 应该只有数字，而 cardId 可能在前面带有 host，形如: `{host} {rankId}`
    dbPath?: string;
    index: number;
}
interface ParseStatisticNotification {
    rankIds: string[];
}
interface ParseLeaksNotification {
    deviceIds: object;
    threadIds: number[];
}

interface ParseHeatmapNotification {
    parseResult: boolean;
}

export const parseMemorySuccessHandler: NotificationInterceptor<ParseMemoryNotification> = (data): void => {
    const session = store.sessionStore.activeSession;
    const currentCardInfos: CardInfo[] = [...session.memoryCardInfos];
    const memoryCardIdSet: Set<string> = new Set(currentCardInfos.map(({ cardId }): string => cardId));
    data.memoryResult.forEach((item) => {
        if (!memoryCardIdSet.has(item.rankId) && item.hasMemory) {
            currentCardInfos.push({ cardId: item.rankId, dbPath: item.dbPath ?? '', index: Number.isNaN(item.rankId) ? 0 : Number(item.rankId) });
            memoryCardIdSet.add(item.rankId);
        }
    });
    updateSession({ memoryCardInfos: currentCardInfos });
};

export const parseOperatorSuccessHandler: NotificationInterceptor<ParseOperatorNotification> = (data): void => {
    const { sessionStore } = store;
    const session = sessionStore.activeSession;
    if (!session) {
        return;
    }
    const ids = [...session.operatorCardInfos, {
        cardId: String(data.rankId),
        dbPath: data.dbPath ?? '',
        index: Number.isNaN(data.rankId) ? 0 : Number(data.rankId),
    } as Required<CardInfo>].sort((a, b) => a.index - b.index);
    updateSession({ operatorCardInfos: ids });
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

export const parseLeaksSuccessHandler: NotificationInterceptor<ParseLeaksNotification> = (data): void => {
    updateSession({ deviceIds: data.deviceIds, threadIds: data.threadIds });
};

export const profilingExpertDataParsedHandler: NotificationInterceptor<ParseHeatmapNotification> = (data): void => {
    const session = store.sessionStore.activeSession;

    runInAction(() => {
        session.profilingExpertDataParsed = data.parseResult ?? false;
    });
};
