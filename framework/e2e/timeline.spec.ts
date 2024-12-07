/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { TimelinePage, SystemView } from './page-object';
import { clearAllData, importData, waitForWebSocketEvent } from './utils';
import { InputHelpers, SelectHelpers } from './components';

interface TestFixtures {
    timelinePage: TimelinePage;
}
const test = baseTest.extend<TestFixtures>({
    timelinePage: async ({ page }, use) => {
        const timelinePage = new TimelinePage(page);
        await use(timelinePage);
    },
});

test.describe('Timeline', () => {
    test.beforeEach(async ({ page, timelinePage }) => {
        const { timelineFrame } = timelinePage;
        await timelinePage.goto();
        await importData(page);
        const secondLayerUnit = timelineFrame.locator('#main-container').getByText('Python (2045554)');
        await expect(secondLayerUnit).toBeVisible();
    });

    // 泳道展开收缩
    test('test_unitsExpandAndCollapse_when_click', async ({ timelinePage }) => {
        const { timelineFrame } = timelinePage;
        const secondLayerUnit = timelineFrame.locator('#main-container').getByText('Python (2045554)');
        const firstUnitInfo = timelineFrame.locator('.unit-info').first();
        await firstUnitInfo.click();
        await expect(secondLayerUnit).toBeHidden();
        await firstUnitInfo.click();
        await expect(secondLayerUnit).toBeVisible();
    });

    // 泳道置顶
    test('test_unitsPinToTopAndUnpin', async ({ timelinePage }) => {
        const { timelineFrame } = timelinePage;
        const firstUnitInfo = timelineFrame.locator('.unit-info').first();
        await firstUnitInfo.hover();
        const pinBtn = timelineFrame.getByTestId('pin-btn');
        await pinBtn.waitFor({ state: 'visible' });
        await pinBtn.click();
        const firstLayerUnit = timelineFrame.locator('#main-container').getByText('0', { exact: true });
        const secondLayerUnit = timelineFrame.locator('#main-container').getByText('Python (2045554)');
        expect(await firstLayerUnit.count()).toBe(2);
        expect(await secondLayerUnit.count()).toBe(2);

        await pinBtn.first().click();
        expect(await firstLayerUnit.count()).toBe(1);
        expect(await secondLayerUnit.count()).toBe(1);
    });

    // 偏移量设置
    test('test_offsetConfig', async ({ page, timelinePage }) => {
        const { timelineFrame } = timelinePage;
        const firstUnitInfo = timelineFrame.locator('.unit-info').first();
        const secondUnitInfo = timelineFrame.locator('.unit-info').nth(1);
        await secondUnitInfo.click();
        await firstUnitInfo.hover({ force: true });
        // 这里需要优化，改为当图表渲染完成后再继续执行
        await page.waitForTimeout(1500);
        const offsetBtn = timelineFrame.getByTestId('offset-btn').first();
        await offsetBtn.click();
        const offsetInput = timelineFrame.getByRole('tooltip', { name: /Timestamp Offset/i }).getByRole('textbox');
        await offsetInput.fill('300000000');
        await offsetInput.press('Enter');
        await offsetBtn.click();
        await page.mouse.move(0, 0);
        await expect(timelineFrame.locator('#main-container')).toHaveScreenshot('unit-offset.png', { maxDiffPixels: 100 });
    });

    // 点击算子展示算子详情
    test('test_showSliceDetail_when_clickOperator', async ({ page, timelinePage }) => {
        const { timelineFrame } = timelinePage;
        const secondUnitInfo = timelineFrame.locator('.unit-info').nth(1);
        await secondUnitInfo.click();
        // 这里需要优化，改为当图表渲染完成后再继续执行
        await page.waitForTimeout(1000);
        // 点击算子
        await timelineFrame
            .locator('.chart > div > .canvasContainer > .drawCanvas')
            .first()
            .click({
                position: {
                    x: 79,
                    y: 9,
                },
            });
        await expect(timelineFrame.getByText('Title')).toBeVisible();
    });

    // 框选泳道展示算子列表
    test('test_showSliceList_when_selectUnitsRange', async ({ page, timelinePage }) => {
        const { timelineFrame } = timelinePage;
        const secondUnitInfo = timelineFrame.locator('.unit-info').nth(1);
        await secondUnitInfo.click();
        const chart = timelineFrame.locator('.chart-selected > div > .canvasContainer > .drawCanvas');
        const chartInfo = await chart.boundingBox();
        if (!chartInfo) {
            return;
        }
        const { x: startX, y: startY } = chartInfo;

        await page.mouse.move(startX + 50, startY + 50);
        await page.mouse.down();
        await page.mouse.move(startX + 200, startX - 200);
        await page.mouse.up();
        await expect(timelineFrame.getByText('Wall Duration', { exact: true })).toBeVisible();
        await expect(timelineFrame.getByText('Self Time')).toBeVisible();
        await expect(timelineFrame.getByText('Average Wall Duration')).toBeVisible();
        const rows = await timelineFrame.locator('.ant-table-row').count();
        expect(rows).toBeGreaterThan(0);
    });
    // System View数据展示
    test('test_SystemViewDataDisplay', async ({ page, timelinePage }) => {
        const { bottomPanel, timelineFrame } = timelinePage;
        const systemView = new SystemView(page);
        await systemView.goto();

        const statsSystemViewOptions = [
            'Python API Summary',
            'CANN API Summary',
            'Ascend HardWare Task Summary',
            'HCCL Summary',
            'Overlap Analysis',
            'Kernel Details',
        ];

        for (let item of statsSystemViewOptions) {
            const option = timelineFrame.getByText(item, { exact: true });
            await option.click();
            await expect(bottomPanel).toHaveScreenshot(`timeline-${item}.png`, { maxDiffPixels: 100 });
        }
    });

    // 算子搜索
    test('test_operatorSearch_when_EnterOperatorName', async ({ page, timelinePage }) => {
        const { searchBtn, timelineFrame } = timelinePage;
        await searchBtn.click();
        const inputLocator = timelineFrame.locator('.insight-category-search-overlay input');
        const input = new InputHelpers(page, inputLocator, timelineFrame);
        await input.setValue('add');
        await input.press('Enter');
        await page.mouse.move(0, 0);
        await page.waitForTimeout(1000);
        await expect(timelineFrame.locator('#main-container')).toHaveScreenshot(`search-operator.png`, { maxDiffPixels: 100 });
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
