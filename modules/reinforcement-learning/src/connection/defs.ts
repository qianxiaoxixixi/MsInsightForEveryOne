/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
export interface NotificationEvent<T = Record<string, unknown>> {
    event: string;
    body: T;
}

export type NotificationHandler<T = Record<string, unknown>> = (body: T) => void;
