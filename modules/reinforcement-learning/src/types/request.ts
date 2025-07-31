/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { DataSource } from '@/types/common';

export interface RequestOptions {
    command: string;
    params?: Record<string, any>;
    module?: string;
}

export interface ResponseBody {
    body: any;
    dataSource: DataSource;
}

export type Request = (options: RequestOptions) => Promise<any>;
