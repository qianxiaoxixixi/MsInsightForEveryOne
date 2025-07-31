/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

export interface GetTraceDataParams {
    framework?: string;
    algorithm?: string;
}

export interface SliceItem {
    type: 'compute_log_prob' | 'generate_sequences' | 'update_actor';
    name: string;
    startTime: number;
    duration: number;
}

export interface TraceDataItem {
    dbPath: string; // db路径，跳转时需要这个信息
    lists: SliceItem[];
    rankId: string; // 纵坐标
}

export interface GetTraceDataResults {
    data: TraceDataItem[];
    minTime: number;
    maxTime: number;
}
