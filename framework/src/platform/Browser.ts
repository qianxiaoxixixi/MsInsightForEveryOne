/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import { BasePlatform } from './BasePlatform';

export class Browser extends BasePlatform {
    selectFolder = (): Promise<string> =>
        new Promise((resolve) => {
            resolve('browser');
        });

    selectFile = (): Promise<string> => this.selectFolder();

    sendMessage = (): void => {};
}
