/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import { ClientConnector } from '../connection';
import { errorCenter, WsError } from './ErrorCenter';

export interface RequestOptions {
    silent?: boolean; // 不显示报错信息
}

/**
 * 创建请求方法
 * @param connector 各模块的 connector 实例
 */
export function createRequest(connector: ClientConnector) {
    return async (
        command: string,
        params: any,
        module?: string,
        options?: RequestOptions,
    ): Promise<any> => {
        try {
            const data = await connector.fetch({
                args: { command, params },
                module: module !== undefined ? module : String(command).split('/')[0]?.toLowerCase(),
            });

            return (data as any)?.body;
        } catch (error: any) {
            const wsError = new WsError(error.code, error.message);
            if (!options?.silent) {
                errorCenter.handleError(wsError);
            }

            throw wsError;
        }
    };
}
