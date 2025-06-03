/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import { runInAction } from 'mobx';
import i18n from 'ascend-i18n';
import { customConsole as console } from 'ascend-utils';
import { store } from '../store';
import type { NotificationHandler } from './defs';
export const parseStatisticCompletedHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        runInAction(() => {
            if (!session) {
                return;
            }
            const rankIds = data.rankIds as string[];
            rankIds.forEach((item) => {
                if (!session.iERankIds.includes(item)) {
                    session.iERankIds.push(item);
                }
            });
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
            session.iERankIds = [];
            session.isCluster = false;
            session.compareRank.rankId = '';
            memorySession.rankIdCondition = { options: [], value: '' };
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
                if (key === 'memoryRankIds') {
                    const memoryRankIds = data.memoryRankIds as string[];
                    session.iERankIds = [...new Set([...session.iERankIds, ...memoryRankIds])];
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
            session.shouldRefresh = true;
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
            const deleteIds: Set<string> = new Set((data.info as Array<{ cardId: string; dbPath: string }>).map(({ cardId }) => cardId));
            if (deleteIds.size > 0) {
                session.iERankIds = session.iERankIds.filter((item: string) => !deleteIds.has(item));
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
            session.shouldRefresh = false;
            session.compareRank = { rankId: data.rankId as string, isCompare: data.isCompare as boolean };
        });
    }
};
