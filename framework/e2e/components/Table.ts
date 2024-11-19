/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { Page, Locator, expect, FrameLocator } from '@playwright/test';
import { Component } from './Component';

export class TableHelpers extends Component {
    // 获取表格行数
    async getTableRows(): Promise<number> {
        return this.locator.locator('tbody tr').count();
    }

    // 获取单元格文本
    async getCellText(row: number, column: number): Promise<string> {
        return this.locator.locator(`tbody tr:nth-child(${row}) td:nth-child(${column})`).innerText();
    }
}
