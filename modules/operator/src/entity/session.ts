/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { makeAutoObservable } from 'mobx';

export interface DirInfo {
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
    allCardInfos: CardInfo[] = [];
    renderId: number = 0;
    total: number = 0;
    isDark: boolean = true;
    // global param
    dirInfo: DirInfo = { rankId: '', isCompare: false };
    constructor() {
        makeAutoObservable(this);
    }
}
