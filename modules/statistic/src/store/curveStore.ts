/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';
import { CurveSession } from '../entity/curveSession';

export class CurveStore {
    private _activeSession: CurveSession | undefined;

    constructor() {
        makeAutoObservable(this);
        this._activeSession = new CurveSession();
    }

    /**
     * The session store is locked if a session is in recording, users can't switch between sessions when session store is locked.
     *
     * @returns Whether session store is locked
     */
    get activeSession(): CurveSession | undefined {
        return this._activeSession;
    }

    set activeSession(value: CurveSession | undefined) {
        this._activeSession = value;
    }

    // creates a new session in the store.
    async newSession(conf?: Partial<CurveSession> | CurveSession): Promise<CurveSession | undefined> {
        return new CurveSession();
    }
};
