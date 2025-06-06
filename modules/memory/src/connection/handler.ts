/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { store } from '../store';
import { runInAction } from 'mobx';
import type { NotificationHandler } from './defs';
import type { RankInfo } from '../entity/memory';
import type { CardInfo } from '../entity/session';
import i18n from 'ascend-i18n';
import { customConsole as console, transformCardIdInfo } from 'ascend-utils';
import { DEFAULT_CARD_VALUE } from '../entity/memorySession';

function addMemoryCardInfos(before: CardInfo[], addList: RankInfo[]): CardInfo[] {
    const current = [...before];
    const currentCardIds: Set<string> = new Set(current.map(({ cardId }) => cardId));
    addList.forEach((item) => {
        if (!currentCardIds.has(item.rankId) && (item.hasMemory as boolean)) {
            const cardIdInfo = transformCardIdInfo(item.rankId);
            current.push({ cardId: item.rankId, dbPath: item.dbPath ?? '', index: cardIdInfo.index });
            currentCardIds.add(item.rankId);
        }
    });
    return current;
}

/** framework 过滤器中也有拦截，逻辑一致 {@link parseMemorySuccessHandler} */
export const parseMemoryCompletedHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        runInAction(() => {
            if (!session) {
                return;
            }
            const memoryResult = data.memoryResult as RankInfo[];
            session.memoryCardInfos = addMemoryCardInfos(session.memoryCardInfos, memoryResult);
        });
    } catch (err) {
        console.error(err);
    }
};

export const removeRemoteHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore, memoryStore } = store;
        const session = sessionStore.activeSession;
        const memorySession = memoryStore.activeSession;
        runInAction(() => {
            if (!session || !memorySession) {
                return;
            }
            session.memoryCardInfos = [];
            session.isAllMemoryCompletedSwitch = false;
            session.isCluster = false;
            session.compareRank.rankId = '';
            memorySession.rankCondition = { options: [], value: DEFAULT_CARD_VALUE };
        });
    } catch (error) {
        console.error(error);
    }
};

export const setTheme: NotificationHandler = (data): void => {
    window.setTheme(Boolean(data.isDark));
};

export const updateSessionHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        runInAction(() => {
            if (!session) {
                return;
            }
            const dataKeys = Object.keys(data);
            const usableKeys: string[] = ['isCluster', 'unitcount'];
            dataKeys.forEach((key: any) => {
                if (key === 'memoryCardInfos') {
                    const memoryCardInfos = data.memoryCardInfos as CardInfo[];
                    // cardId 不能转成数字时排序不会报错，但是会出现逻辑错误（排序失效）
                    session.memoryCardInfos = [...memoryCardInfos].sort((a, b) => Number(a.index) - Number(b.index));
                    return;
                }
                if (usableKeys.includes(key)) {
                    (session as any)[key] = data[key];
                }
            });
        });
    } catch (error) {
        console.error(error);
    }
};

export const allSuccessHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        runInAction(() => {
            if (!session) {
                return;
            }
            if (data.isAllPageParsed as boolean) {
                session.isAllMemoryCompletedSwitch = !session.isAllMemoryCompletedSwitch;
            }
        });
    } catch (error) {
        console.error(error);
    }
};

export const deleteCardHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        runInAction(() => {
            if (!session) {
                return;
            }
            const deleteIds: Set<string> = new Set((data.info as CardInfo[]).map(({ cardId }) => cardId));
            if (deleteIds.size > 0) {
                session.memoryCardInfos = session.memoryCardInfos.filter((item) => !deleteIds.has(item.cardId));
            }
        });
    } catch (error) {
        console.error(error);
    }
};

export const switchLanguageHandler: NotificationHandler = (data): void => {
    const session = store.sessionStore.activeSession;
    const lang = data.lang as 'zhCN' | 'enUS';
    if (session) {
        runInAction(() => {
            session.language = lang;
        });
    }
    i18n.changeLanguage(lang);
};

export const switchDirectoryHandler: NotificationHandler = (data): void => {
    const session = store.sessionStore.activeSession;
    if (session) {
        runInAction(() => {
            session.compareRank = { rankId: data.rankId as string, isCompare: data.isCompare as boolean };
        });
    }
};
