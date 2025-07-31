/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { connector } from '@/connection';
import type { Request, ResponseBody } from '@/types/request';
import { message } from 'antd';

export const request: Request = async ({ command, params, module }) => {
    const moduleName = module ?? command.split('/')?.[0]?.toLowerCase() ?? '';

    try {
        const res = await connector.fetch({
            args: {
                command,
                params,
            },
            module: moduleName,
        });

        const result = res as ResponseBody;

        return result.body;
    } catch (error: any) {
        message.error(error.message || '请求异常');
        throw error;
    }
};
