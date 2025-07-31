/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import { NotificationEvent, NotificationHandler } from '@/connection/defs';
import { connector } from '@/connection';

function isValidPayload(payload: unknown): payload is Record<string, unknown> {
    return typeof payload === 'object' && payload !== null;
}

export function registerMessageListeners(handlers: Record<string, NotificationHandler>): void {
    Object.entries(handlers).forEach(([event, handler]) => {
        connector.addListener(event, (e: MessageEvent<NotificationEvent>) => {
            const { body } = e.data;

            if (!isValidPayload(body)) return;
            handler(body);
        });
    });
}
