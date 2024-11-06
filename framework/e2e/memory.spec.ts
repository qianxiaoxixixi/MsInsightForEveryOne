/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { MemoryPage } from './page-object';
import { clearAllData, importData, waitForWebSocketEvent } from './utils';
import { SelectHelpers } from './components';

interface TestFixtures {
    memoryPage: MemoryPage;
}
const test = baseTest.extend<TestFixtures>({
    memoryPage: async ({ page }, use) => {
        const memoryPage = new MemoryPage(page);
        await use(memoryPage);
    },
});

test.describe('Memory', () => {
    test.beforeEach(async ({ page, memoryPage }) => {
        const allCardParsedPromise = waitForWebSocketEvent(page, (res) => res?.event === 'allPagesSuccess');
        await memoryPage.goto();
        await importData(page);
        await allCardParsedPromise;
    });

    test('页面加载成功', async ({ page, memoryPage }) => {
        const { memoryFrame } = memoryPage;
        await expect(memoryFrame.locator('.mi-page')).toHaveScreenshot('memory.png');
    });

    test('筛选卡序号', async ({ page, memoryPage }) => {
        const { memoryFrame, rankIdSelector } = memoryPage;
        const rankIdSelect = new SelectHelpers(page, rankIdSelector, memoryFrame);
        await rankIdSelect.open();
        await rankIdSelect.selectOption('4');
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
