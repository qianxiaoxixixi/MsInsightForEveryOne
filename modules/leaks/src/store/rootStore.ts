/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
// Leaks Module
import { SessionStore } from './session';

export class RootStore {
    sessionStore: SessionStore;

    constructor() {
        this.sessionStore = new SessionStore();
    }

    resetStore = (): void => {
        this.sessionStore = new SessionStore();
    };
}

export const store = new RootStore();
