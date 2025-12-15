/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import React from 'react';
import { message, Modal } from 'antd';

export enum ErrorCode {
    WS_ERROR = 'WS_ERROR',
    PARSE_FAIL = 'PARSE_FAIL',
    IMPORT_ERROR = 4112,
}

export class WsError extends Error {
    code: ErrorCode | string | number;

    constructor(code: number | string, message: string) {
        super(message);
        this.code = code;
        this.name = 'WsError';
    }
}

class ErrorCenter {
    private lastWsErrorTime = 0;

    handleError(err: WsError): void {
        switch (err.code) {
            case ErrorCode.WS_ERROR:
            case ErrorCode.PARSE_FAIL:
                if (this.shouldThrottle()) return;
                message.error(err.message);
                break;
            case ErrorCode.IMPORT_ERROR:
                Modal.error({
                    title: err.message.split(';')[0],
                    content: (
                        <>
                            {

                                err.message.split(';').slice(1).filter(Boolean).map((line, index) => <div style={{ padding: 2 }} key={index}>{line.trim()}</div>)
                            }
                        </>
                    ),
                    closable: true,
                });
                break;
            default:
                message.error(err.message ?? 'Unknown error');
        }
    }

    private shouldThrottle(): boolean {
        const now = Date.now();
        if (now - this.lastWsErrorTime < 1000) return true; // 1 秒内不重复提示
        this.lastWsErrorTime = now;
        return false;
    }
}

export const errorCenter = new ErrorCenter();
