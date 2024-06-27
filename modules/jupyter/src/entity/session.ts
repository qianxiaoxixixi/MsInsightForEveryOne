/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import { makeAutoObservable } from 'mobx';

export class Session {
    language: 'zhCN' | 'enUS' = 'enUS';
    isIpynb: boolean = false;
    ipynbUrl: string = '';
    constructor() {
        makeAutoObservable(this);
    }
}
