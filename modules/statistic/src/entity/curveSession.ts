/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';
import { DataDetail } from './curve';

export interface ConditionType {
    options: string[];
    value: string;
    ranks?: Map<string, string[]>;
};

export interface SelectedRange {
    startTs: string;
    endTs: string;
};

export class CurveSession {
    // 顶部筛选条件相关变量
    rankIdCondition: ConditionType = { options: [], value: '' };
    groupCondition: Array<{ label: string; value: string }> = [];
    groupId: string = '';
    // 中部折线图框选和下方表格联动
    selectedRange?: SelectedRange;
    isBtnDisabled: boolean = true;
    selectedRecord?: DataDetail;
    // 底部表格分页相关变量
    current: number = 1;
    pageSize: number = 10;

    constructor() {
        makeAutoObservable(this);
    }
}
