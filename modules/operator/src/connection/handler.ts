/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { store } from '../store';
import { runInAction } from 'mobx';
import type { NotificationHandler } from './defs';
import i18n from 'ascend-i18n';
import type { CardInfo, DirInfo } from '../entity/session';
import { transformCardIdInfo } from 'ascend-utils';

export const setTheme: NotificationHandler = (data): void => {
    window.setTheme(Boolean(data.isDark));
};

export const updateSessionHandler: NotificationHandler = (data): void => {
    const session = store.sessionStore.activeSession;
    runInAction(() => {
        if (!session) {
            return;
        }
        const dataKeys = Object.keys(data);
        const sessionKeys = Object.keys(session);
        dataKeys.forEach((key: any) => {
            if (sessionKeys.includes(key)) {
                (session as any)[key] = data[key];
            }
            if (key === 'operatorCardInfos') {
                session.allCardInfos = [...data[key] as CardInfo[]].sort((a, b) => Number(a.index) - Number(b.index));
            }
        });
        session.renderId = session.renderId++ % 1000;
    });
};

export const switchDirectoryHandler: NotificationHandler = async (data): Promise<void> => {
    const session = store.sessionStore.activeSession;
    const dirInfo = { rankId: data.rankId, isCompare: data.isCompare } as DirInfo;
    runInAction(() => {
        if (!session) {
            return;
        }
        session.dirInfo = dirInfo;
    });
};

/** framework 过滤器中也有拦截，逻辑一致 {@link parseOperatorSuccessHandler} */
export const parseSuccessHandler: NotificationHandler = (data): void => {
    const { sessionStore } = store;
    const session = sessionStore.activeSession;
    runInAction(() => {
        if (!session) {
            return;
        }
        if (session.allCardInfos.every(({ cardId }) => cardId !== String(data.rankId))) {
            const cardIdInfo = transformCardIdInfo(String(data.rankId));
            session.allCardInfos = [...session.allCardInfos, {
                cardId: String(data.rankId),
                dbPath: data.dbPath ?? '',
                index: cardIdInfo.index,
            } as CardInfo].sort((a, b) => a.index - b.index);
        }
    });
};

export const resetHandler: NotificationHandler = (data): void => {
    const { sessionStore } = store;
    const session = sessionStore.activeSession;
    runInAction(() => {
        if (!session) {
            return;
        }
        session.allCardInfos = [];
    });
};

export const deleteCardHandler: NotificationHandler = (data): void => {
    const { sessionStore } = store;
    const session = sessionStore.activeSession;
    runInAction(() => {
        if (!session) {
            return;
        }
        const deleteIds: Set<string> = new Set((data.info as any[]).map(({ cardId }) => cardId));
        if (deleteIds.size > 0) {
            session.allCardInfos = session.allCardInfos.filter((item) => !deleteIds.has(item.cardId));
        }
    });
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
