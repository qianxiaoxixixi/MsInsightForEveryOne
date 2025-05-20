/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';

export interface ICompareRankInfo {
    rankId: string;
    isCompare: boolean;
}

export class Session {
    language: 'zhCN' | 'enUS' = 'enUS';
    unitcount = 0;
    iERankIds: string[] = [];
    isCluster: boolean = false;
    shouldRefresh: boolean = true;
    compareRank: ICompareRankInfo = { rankId: '', isCompare: false };

    constructor() {
        makeAutoObservable(this);
    }
}
