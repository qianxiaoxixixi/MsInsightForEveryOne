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

import { request } from '@/utils/request';
import type { MemoryLifeResponse, MemoryUsageResponse, BasicInfoResponse } from './types';

export const getBasicInfo = (): Promise<BasicInfoResponse> => {
    return request('Triton/memory/basicInfo', {});
};

export const getMemoryLife = async (scope: string): Promise<MemoryLifeResponse> => {
    return await request('Triton/memory/blocks', { scope });
};

export const getMemoryUsage = async (timestamp: number, scope: string): Promise<MemoryUsageResponse> => {
    return request('Triton/memory/usage', { timestamp, scope });
};
