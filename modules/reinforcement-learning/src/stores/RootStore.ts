/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';
import { TraceStore } from '@/stores/TraceStore';
import { SessionStore } from '@/stores/SessionStore';

export class RootStore {
    traceStore: TraceStore;
    sessionStore: SessionStore;

    constructor() {
        this.traceStore = new TraceStore(this);
        this.sessionStore = new SessionStore(this);

        makeAutoObservable(this, {}, { autoBind: true });
    }

    reset(): void {
        this.sessionStore.reset();
        this.traceStore.reset();
    }
}

export const rootStore = new RootStore();
