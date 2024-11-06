/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import type { FrameLocator, Page } from '@playwright/test';
import { FrameworkPage } from './framework';

export class OperatorPage {
    readonly page: Page;
    readonly operatorFrame: FrameLocator;

    constructor(page: Page) {
        this.page = page;
        this.operatorFrame = page.frameLocator('#Operator');
    }

    async goto(): Promise<void> {
        const frameworkPage = new FrameworkPage(this.page);
        await frameworkPage.goto();
        await frameworkPage.clickTab('operator');
    }
}
