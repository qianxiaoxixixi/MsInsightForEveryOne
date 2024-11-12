/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { CommunicationPage } from './page-object';
import { clearAllData, importData } from './utils';
import { SelectHelpers, CheckboxHelpers } from './components';

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
        await page.goto('/');
        await importData(page);
        await communicationPage.goto();
    });

    // 筛选迭代id
    test('filter iteration ID', async ({ page, communicationPage }) => {
        const { stepSelector, communicationFrame } = communicationPage;
        const stepSelect = new SelectHelpers(page, stepSelector, communicationFrame);
        await stepSelect.open();
        await stepSelect.selectOption('3');
    });

    // 筛选通信域
    test('filter communication group', async ({ page, communicationPage }) => {
        const { communicationGroupSelector, communicationFrame } = communicationPage;
        const communicationGroupSelect = new SelectHelpers(page, communicationGroupSelector, communicationFrame);
        await communicationGroupSelect.open();
        await communicationGroupSelect.selectOption('(0, 1, 2, 3, 4, 5, 6, 7)');
    });

    // 筛选算子名称
    test('filter operator name', async ({ page, communicationPage }) => {
        const { operatorNameSelector, communicationFrame } = communicationPage;
        const operatorNameSelect = new SelectHelpers(page, operatorNameSelector, communicationFrame);
        await operatorNameSelect.open();
        await operatorNameSelect.selectOption('allgather-top1');
    });

    // 通信矩阵
    test('select communication matrix', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, communicationFrame, communicationMatrixTypeSelector } = communicationPage;
        await communicationMatrixRadio.check();
        const communicationMatrixTypeSelect = new SelectHelpers(page, communicationMatrixTypeSelector, communicationFrame);
        await communicationMatrixTypeSelect.open();
        await communicationMatrixTypeSelect.selectOption('Transit Size(MB)');
        const showInnerCommunicationCheckbox = communicationFrame.getByTestId('showInnerCommunication');
        await showInnerCommunicationCheckbox.check();
        await page.mouse.move(0, 0);
        await expect(communicationFrame.locator('#matrixchart')).toHaveScreenshot('communicationMatrix.png');
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
