/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { NotificationHandler } from '@/connection/defs';
import { rootStore, SessionStore } from '@/stores';
import { runInAction } from 'mobx';

export const updateSessionHandler: NotificationHandler<Partial<SessionStore>> = (body): void => {
    const { sessionStore } = rootStore;

    runInAction(() => {
        Object.entries(body).forEach(([key, value]) => {
            sessionStore.updateField(key as keyof SessionStore, value);
        });
    });
};
