/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

declare global {
    interface Window {
        requestData: (method: string, params: any, module?: string, requestOptions?: RequestOptions) => Promise<any>;
    }
}

export {};
