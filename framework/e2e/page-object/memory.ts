/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import type { FrameLocator, Locator, Page } from '@playwright/test';
import { FrameworkPage } from './framework';

export class MemoryPage {
    readonly page: Page;
    readonly memoryFrame: FrameLocator;
    readonly rankIdSelector: Locator;

    constructor(page: Page) {
        this.page = page;
        this.memoryFrame = page.frameLocator('#Memory');
        this.rankIdSelector = this.memoryFrame.locator('.ant-select[name="rankId"]');
    }

    async goto(): Promise<void> {
        const frameworkPage = new FrameworkPage(this.page);
        await frameworkPage.goto();
        await frameworkPage.clickTab('memory');
    }
}
