/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 */

export interface GraphParam {
    startTimestamp?: number;
    endTimestamp?: number;
}

/**
 * 获取块图信息
 * @param params 查询条件
 * @returns 查询结果
 */
export const getBlocksGraphData = async (params: GraphParam): Promise<Omit<RenderData, 'block'>> => {
    return window.request('Triton/memory/blocks', { ...params });
};

export const getMemoryStateData = async (params: { timestamp: number }): Promise<{ segments: Segment[] }> => {
    return window.request('Triton/memory/usage', { ...params });
};
