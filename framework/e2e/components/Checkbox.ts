/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { Component } from './Component';

export class CheckboxHelpers extends Component {
    // 点击复选框
    async click(): Promise<void> {
        const checkBox = this.locator.locator('xpath=../..');
        await checkBox?.click();
    }
}
