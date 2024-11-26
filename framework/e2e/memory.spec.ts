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

const memoryImgMap = {
    loadPytorchSingleMachineMultiRankDataSuccess: 'memory-pytorch-single.png',
    filterPytorchSingleMachineMultiRankDataSuccess: 'memory-pytorch-single-filter.png',
};
 
test.describe('Memory(Pytorch_SingleMachineMultiRankData)', () => {
    test.beforeEach(async ({ page, memoryPage }) => {
        const allCardParsedPromise = waitForWebSocketEvent(page, (res) => res?.event === 'allPagesSuccess');
        await memoryPage.goto();
        await importData(page);
        await allCardParsedPromise;
    });

    test('loadMemoryPageSuccess_when_dataParseSuccess', async ({ page, memoryPage }) => {
        const { memoryFrame } = memoryPage;
        await page.mouse.move(0, 0);
        await expect(memoryFrame.locator('.mi-page'))
        .toHaveScreenshot(memoryImgMap.loadPytorchSingleMachineMultiRankDataSuccess, {
            maxDiffPixels: 500,
        });
    });
 
    test('change_filterCondition', async ({ page, memoryPage }) => {
        const { memoryFrame, rankIdSelector, groupIdSelector } = memoryPage;
        const rankIdSelect = new SelectHelpers(page, rankIdSelector, memoryFrame);
        const groupIdSelect = new SelectHelpers(page, groupIdSelector, memoryFrame);
        await rankIdSelect.open();
        await rankIdSelect.selectOption('4');
        await groupIdSelect.open();
        await groupIdSelect.selectOption('Stream');
        await page.mouse.move(0, 0);
        await expect(memoryFrame.locator('.mi-page'))
        .toHaveScreenshot(memoryImgMap.filterPytorchSingleMachineMultiRankDataSuccess, {
            maxDiffPixels: 500,
        });
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
