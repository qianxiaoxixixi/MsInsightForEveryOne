/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
export interface BarData {
    blockData: BlockData;
    allocationData: AllocationData;
    isDark: boolean;
};
export interface BlockData {
    minTimestamp: number;
    maxTimestamp: number;
    minSize: number;
    maxSize: number;
    blocks: Block[];
};
export interface Block {
    id: number;
    addr: string;
    size: number;
    startTimestamp: number;
    endTimestamp: number;
    owner: string;
    attr: string;
    path: number[][];
}
export interface AllocationData {
    minTimestamp: number;
    maxTimestamp: number;
    allocations: Allocation[];
}
export interface Allocation {
    id: number;
    timestamp: number;
    totalSize: number;
}

/**
 * 获取图表信息
 * @param params 查询条件
 * @returns 查询结果
 */
export const GetLeaksGraphData = async ({ deviceId, graph }: { deviceId: string; graph: string }): Promise<BlockData | AllocationData> => {
    return window.request({ command: `Memory/leaks/${graph}`, params: { deviceId, relativeTime: true } });
};
