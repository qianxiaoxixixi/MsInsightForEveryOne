/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { SummaryPage } from './page-object';
import { clearAllData, importData } from './utils';

interface TestFixtures {
    summaryPage: SummaryPage;
}
const test = baseTest.extend<TestFixtures>({
    summaryPage: async ({ page }, use) => {
        const summaryPage = new SummaryPage(page);
        await use(summaryPage);
    },
});

test.describe('Summary', () => {
    test.beforeEach(async ({ page, summaryPage }) => {
        const { summaryFrame } = summaryPage;
        await importData(page);
        await summaryPage.goto();
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
