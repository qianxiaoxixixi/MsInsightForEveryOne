/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest } from '@playwright/test';
import { CommunicationPage } from './page-object';
import { clearAllData, importData } from './utils';

interface TestFixtures {
    communicationPage: CommunicationPage;
}
const test = baseTest.extend<TestFixtures>({
    communicationPage: async ({ page }, use) => {
        const communicationPage = new CommunicationPage(page);
        await use(communicationPage);
    },
});

test.describe('Communication', () => {
    test.beforeEach(async ({ page, communicationPage }) => {
        await importData(page);
        await communicationPage.goto();
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
