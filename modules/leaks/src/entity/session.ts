/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
import { makeAutoObservable } from 'mobx';

export class Session {
    language: 'zhCN' | 'enUS' = 'enUS';
    deviceIds: string[] = [];
    constructor() {
        makeAutoObservable(this);
    }
}
