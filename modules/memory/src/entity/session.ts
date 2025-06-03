/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';

export interface ICompareRankInfo {
    rankId: string;
    isCompare: boolean;
}

export interface CardInfo {
    cardId: string;
    dbPath: string;
    index: number;
}

export class Session {
    language: 'zhCN' | 'enUS' = 'enUS';
    unitcount = 0;
    memoryCardInfos: CardInfo[] = [];
    isCluster: boolean = false;
    isAllMemoryCompletedSwitch: boolean = false;
    // rankId 实际是 cardId: `{host} {rankId}`
    compareRank: ICompareRankInfo = { rankId: '', isCompare: false };

    constructor() {
        makeAutoObservable(this);
    }
}
