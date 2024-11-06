/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import type { FrameLocator, Page } from '@playwright/test';
import { FrameworkPage } from './framework';

export class SummaryPage {
    readonly page: Page;
    readonly summaryFrame: FrameLocator;

    constructor(page: Page) {
        this.page = page;
        this.summaryFrame = page.frameLocator('#Summary');
    }

    async goto(): Promise<void> {
        const frameworkPage = new FrameworkPage(this.page);
        await frameworkPage.clickTab('summary');
    }
}
