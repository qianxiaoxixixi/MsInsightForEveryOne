/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import type { NotificationInterceptor, ResponseInterceptor } from './defs';
import {
    type ImportActionResponse,
    parseMemorySuccessHandler,
    parseStatisticSuccessHandler,
    parseLeaksSuccessHandler,
    parseOperatorSuccessHandler,
    profilingExpertDataParsedHandler,
} from './interceptorHandler';

const MEMORY_COMPLETED = 'parse/memoryCompleted';
const STATISTIC_COMPLETED = 'parse/statisticCompleted';
const LEAKS_COMPLETED = 'parse/leaksMemoryCompleted';
const OPERATOR_COMPLETED = 'parse/operatorCompleted';
const PARSE_HEATMAP_COMPLETED = 'parse/heatmapCompleted';

export type ResponseType = ImportActionResponse;
export const INTERCEPTOR_HANDLERS: Record<string, ResponseInterceptor<ResponseType>> = {};

export const NOTIFICATION_INTERCEPTOR_HANDLERS: Record<string, NotificationInterceptor<any>> = {
    [MEMORY_COMPLETED]: parseMemorySuccessHandler,
    [LEAKS_COMPLETED]: parseLeaksSuccessHandler,
    [OPERATOR_COMPLETED]: parseOperatorSuccessHandler,
    [PARSE_HEATMAP_COMPLETED]: profilingExpertDataParsedHandler,
};

export const NOTIFICATION_STATISTIC_HANDLERS: Record<string, NotificationInterceptor<any>> = {
    [STATISTIC_COMPLETED]: parseStatisticSuccessHandler,
};
