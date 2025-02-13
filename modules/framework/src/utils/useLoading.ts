/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { store } from '@/store';
import { runInAction } from 'mobx';

// 整个页面的loading遮盖
const open = (): void => {
    const session = store.sessionStore.activeSession;
    runInAction(() => {
        session.loading = true;
    });
};

const close = (): void => {
    const session = store.sessionStore.activeSession;
    runInAction(() => {
        session.loading = false;
    });
};

function status(): boolean {
    const session = store.sessionStore.activeSession;

    return session.loading;
}

export { open as openLoading, close as closeLoading, status as loadingStatus };
