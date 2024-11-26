/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { OperatorPage } from './page-object';
import { clearAllData, importData, waitForWebSocketEvent } from './utils';
import { SelectHelpers } from './components';

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

    test('change_filterCondition', async ({ page, operatorPage }) => {
        const { operatorFrame, groupIdSelector, rankIdSelector, topSelector } = operatorPage;
        const groupIdSelect = new SelectHelpers(page, groupIdSelector, operatorFrame);
        const rankIdSelect = new SelectHelpers(page, rankIdSelector, operatorFrame);
        const topSelect = new SelectHelpers(page, topSelector, operatorFrame);
        await groupIdSelect.open();
        await groupIdSelect.selectOption('Computing Operator Type');
        await rankIdSelect.open();
        await rankIdSelect.selectOption('4');
        await topSelect.open();
        await topSelect.selectOption('15');
        await page.mouse.move(0, 0);
        await expect(operatorFrame.locator('.mi-page')).toHaveScreenshot('operator.png', {
            maxDiffPixels: 500,
        });
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
