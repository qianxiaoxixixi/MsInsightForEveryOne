/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { request } from '@/utils/request';
import type { GetTraceDataParams, GetTraceDataResults } from '@/api/types';

export const getTraceData = (params: GetTraceDataParams): Promise<GetTraceDataResults> => {
    return request({ command: 'RL/pipeline', params });
};
