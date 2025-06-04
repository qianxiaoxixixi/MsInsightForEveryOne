/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
import { makeAutoObservable } from 'mobx';
export class Session {
    language: 'zhCN' | 'enUS' = 'enUS';
    deviceIds: any = {};
    threadIds: number[] = [];
    blockData: any = { blocks: [], minSize: 0, maxSize: 0, minTimestamp: 0, maxTimestamp: 0 };
    allocationData: any = { allocations: [], maxTimestamp: 0, minTimestamp: 0 };
    memoryData: any = { size: 0, name: '', subNodes: [] };
    funcData: any = [];
    memoryStamp: number = 0;
    deviceId: string = '';
    eventType: string = '';
    threadId: number = 0;
    maxTime: number = 0;
    minTime: number = 0;
    constructor() {
        makeAutoObservable(this);
    }
}
