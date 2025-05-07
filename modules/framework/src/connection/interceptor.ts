/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import type { NotificationInterceptor, ResponseInterceptor } from './defs';
import {
    type ImportActionResponse,
    parseMemorySuccessHandler,
} from './interceptorHandler';

const MEMORY_COMPLETED = 'parse/memoryCompleted';

export type ResponseType = ImportActionResponse;
export const INTERCEPTOR_HANDLERS: Record<string, ResponseInterceptor<ResponseType>> = {};

export const NOTIFICATION_INTERCEPTOR_HANDLERS: Record<string, NotificationInterceptor<any>> = {
    [MEMORY_COMPLETED]: parseMemorySuccessHandler,
};
