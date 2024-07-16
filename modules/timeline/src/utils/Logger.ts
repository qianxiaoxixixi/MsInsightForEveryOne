/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
export type LogLevel = 'debug' | 'info' | 'warn' | 'error';
export const logger = (tag: string, msg: string, level?: LogLevel): void => {
    // wedge platform Logger
};
