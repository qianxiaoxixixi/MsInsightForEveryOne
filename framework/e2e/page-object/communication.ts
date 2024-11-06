/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import type { FrameLocator, Page } from '@playwright/test';
import { FrameworkPage } from './framework';

export class CommunicationPage {
    readonly page: Page;
    readonly communicationFrame: FrameLocator;

    constructor(page: Page) {
        this.page = page;
        this.communicationFrame = page.frameLocator('#Communication');
    }

    async goto(): Promise<void> {
        const frameworkPage = new FrameworkPage(this.page);
        await frameworkPage.clickTab('communication');
    }
}
