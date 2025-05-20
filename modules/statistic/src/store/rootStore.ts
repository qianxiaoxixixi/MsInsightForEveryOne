/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import { SessionStore } from './sessionStore';
import { CurveStore } from './curveStore';

export class RootStore {
    sessionStore: SessionStore;
    memoryStore: CurveStore;

    constructor() {
        this.sessionStore = new SessionStore();
        this.memoryStore = new CurveStore();
    }

    resetStore = (): void => {
        this.sessionStore = new SessionStore();
        this.memoryStore = new CurveStore();
    };
}

export const store = new RootStore();
