/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
// Cluster Module
import { Session } from '../entity/session';

export interface Request {
    id: number;
    method: string;
    params: Record<string, unknown>;
}

export interface Response<T = Record<string, unknown>> {
    id: number;
    result?: T;
    error?: {
        code: number;
        message: string;
    };
}

export interface Notification<T = Record<string, unknown>> {
    method: string;
    params: T;
}

export type NotificationHandler<T = Record<string, unknown>> = (notification: T, session: Session) => void;
