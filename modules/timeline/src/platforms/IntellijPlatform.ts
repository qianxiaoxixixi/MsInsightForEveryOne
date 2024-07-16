/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import type { IMessageSender } from '../connection/messageSender';

export class IntellijPlatform implements IMessageSender {
    selectFolder(): Promise<string> {
        return new Promise(resolve => {
            this.sendMessage({
                request: JSON.stringify({
                    key: 'ascend.selectFolder',
                    data: {
                        method: 'ascend.selectFolder',
                        params: {},
                    },
                }),
                onSuccess: function (response: string) {
                    resolve(JSON.parse(response).body);
                },
            });
        });
    }

    selectFile(): Promise<string> {
        return new Promise(resolve => {
            this.sendMessage({
                request: JSON.stringify({
                    key: 'ascend.selectFolder',
                    data: {
                        method: 'ascend.selectFile',
                        params: {},
                    },
                }),
                onSuccess: function (response: string) {
                    resolve(JSON.parse(response).body);
                },
            });
        });
    }

    sendMessage = (ceq: any): void => {
        window.cefQuery(ceq);
    };
}
