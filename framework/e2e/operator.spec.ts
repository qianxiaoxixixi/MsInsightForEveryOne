/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { OperatorPage } from './page-object';
import { clearAllData, importData, waitForWebSocketEvent } from './utils';

interface TestFixtures {
    operatorPage: OperatorPage;
}
const test = baseTest.extend<TestFixtures>({
    operatorPage: async ({ page }, use) => {
        const memoryPage = new OperatorPage(page);
        await use(memoryPage);
    },
});

test.describe('Operator', () => {
    test.beforeEach(async ({ page, operatorPage }) => {
        const allCardParsedPromise = waitForWebSocketEvent(page, (res) => res?.event === 'allPagesSuccess');
        await operatorPage.goto();
        await importData(page);
        await allCardParsedPromise;
    });

    test('页面加载成功', async ({ page, operatorPage }) => {
        const { operatorFrame } = operatorPage;
        await expect(operatorFrame.locator('.mi-page')).toHaveScreenshot('operator.png');
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
