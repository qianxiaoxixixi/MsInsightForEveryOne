/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 */
import { store } from '../store';
import { runInAction } from 'mobx';
import type { NotificationHandler } from './defs';
import i18n from '@insight/lib/i18n';
import { workerDestroy } from '@/leaksWorker/blockWorker/worker';
import { errorCenter, ErrorCode, WsError } from '@insight/lib';
import { workerDestroy as stateWorkerDestroy } from '@/leaksWorker/stateWorker/worker';

export const setTheme: NotificationHandler = (data): void => {
    window.setTheme(Boolean(data.isDark));
};

export const updateSessionHandler: NotificationHandler = (data): void => {
    const { sessionStore } = store;
    const session = sessionStore.activeSession;
    runInAction(() => {
        if (!session || typeof session !== 'object' || typeof data !== 'object') {
            return;
        }
        const dataKeys = Object.keys(data);
        const sessionKeys = Object.keys(session);
        dataKeys.forEach((key: string) => {
            if (sessionKeys.includes(key)) {
                (session as unknown as Record<string, unknown>)[key] = data[key];
            }
        });
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
const commonRestore = (session: any): void => {
    session.maxTime = 0;
    session.minTime = 0;
    session.legendSelect = {};
    session.synStartTime = 0;
    session.synEndTime = 0;
};
const barRestore = (session: any): void => {
    session.allocationData = { allocations: [], maxTimestamp: 0, minTimestamp: 0 };
    session.blockData = { blocks: [], minSize: 0, maxSize: 0, minTimestamp: 0, maxTimestamp: 0 };
    session.firstOffset = 0;
    session.lastOffset = 0;
    session.markLineshow = 'none';
    session.contextMenu = { visible: false, xPos: 0, yPos: 0 };
    session.allowMark = true;
    session.menuItems = [];
    session.firstLastStamps = { first: 0, last: 0 };
};
const detailsRestore = (session: any): void => {
    session.tableType = 'blocks';
    session.tableKey = (session.tableKey + 1) % 10;
};
const restore = (session: any): void => {
    commonRestore(session);
    barRestore(session);
    detailsRestore(session);
};
export const parseCompletedHandler = (data: any): void => {
    const session = store.sessionStore.activeSession;
    workerDestroy();
    if (session) {
        runInAction(() => {
            restore(session);
        });
    }
};
export const removeRemoteHandler: NotificationHandler = (data): void => {
    const session = store.sessionStore.activeSession;
    workerDestroy();
    stateWorkerDestroy();
    if (session) {
        runInAction(() => {
            session.deviceIds = {};
            restore(session);
        });
    }
};

export const parseFailHandler: NotificationHandler = (data): void => {
    errorCenter.handleError(new WsError(ErrorCode.PARSE_FAIL, data.error as string));
};
