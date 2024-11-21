/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test as baseTest, expect } from '@playwright/test';
import { CommunicationPage, FrameworkPage } from './page-object';
import { clearAllData, importData } from './utils';
import { SelectHelpers, CheckboxHelpers, TableHelpers } from './components';

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
        const { loadingDialog } = new FrameworkPage(page);
        const { fullmask } = communicationPage;
        await page.goto('/');
        await importData(page);
        await communicationPage.goto();
        if (await loadingDialog.count()) {
            await loadingDialog.waitFor({ state: 'attached' });
        }
    });

    // 【case】数据展示配置
    test('data display configuration', async ({ page, communicationPage }) => {
        const {
            stepSelector,
            communicationFrame,
            communicationGroupSelector,
            operatorNameSelector,
            switchDurationAnalysis,
            communicationMatrixRadio,
            durationAnalysisRadio,
        } = communicationPage;
        // 筛选迭代id
        const stepSelect = new SelectHelpers(page, stepSelector, communicationFrame);
        await stepSelect.open();
        await stepSelect.selectOption('3');
        // 筛选通信域
        const communicationGroupSelect = new SelectHelpers(page, communicationGroupSelector, communicationFrame);
        await communicationGroupSelect.open();
        await communicationGroupSelect.selectOption('(0, 1, 2, 3, 4, 5, 6, 7)');
        // 筛选算子名称
        const operatorNameSelect = new SelectHelpers(page, operatorNameSelector, communicationFrame);
        await operatorNameSelect.open();
        await operatorNameSelect.selectOption('allgather-top1');
        // 通信矩阵和通信耗时分析单选开关
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
    });

    // 【case】通信矩阵配置
    test('communication matrix configuration', async ({ page, communicationPage }) => {
        const {
            communicationFrame,
            communicationMatrixRadio,
            communicationMatrixTypeSelector,
            communicationMatrixMaxRangeInput,
            communicationMatrixMinRangeInput,
            matrixChart,
        } = communicationPage;
        // 通信矩阵类型下拉选择
        await communicationMatrixRadio.check();
        const communicationMatrixTypeSelect = new SelectHelpers(page, communicationMatrixTypeSelector, communicationFrame);
        await communicationMatrixTypeSelect.open();
        await communicationMatrixTypeSelect.selectOption('Bandwidth(GB/s)'); // 带宽
        await communicationMatrixTypeSelect.open();
        await communicationMatrixTypeSelect.selectOption('Transit Size(MB)'); // 传输大小
        await communicationMatrixTypeSelect.open();
        await communicationMatrixTypeSelect.selectOption('Transport Type'); // 链路方式
        expect(communicationMatrixMinRangeInput).toBeHidden(); // 切换到链路方式，筛选范围不可见
        await communicationMatrixTypeSelect.open();
        await communicationMatrixTypeSelect.selectOption('Transit Time(ms)'); // 传输时长
        // 显示卡内通信开关（默认：不显示）
        const showInnerCommunicationCheckbox = communicationFrame.getByTestId('showInnerCommunication');
        await expect(showInnerCommunicationCheckbox).not.toBeChecked();
        // 勾选显示卡内通信
        await showInnerCommunicationCheckbox.check();
        // 根据输入控制所显示数据的范围并截图对比
        await communicationMatrixMinRangeInput.press('ArrowUp');
        await communicationMatrixMaxRangeInput.press('ArrowDown');
        const confrimBtn = communicationFrame.locator('button.ant-btn-primary').first();
        await confrimBtn.click();
        await page.mouse.move(0, 0);
        await page.waitForTimeout(1000);
        await expect(matrixChart).toHaveScreenshot('communication-matrix.png');
    });

    // 【case】HCCL图表
    test('HCCL chart', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        await page.mouse.move(0, 0);
        await expect(communicationFrame.locator('#hccl')).toHaveScreenshot('communication-hccl.png');
    });

    // 【case】通信时长图表：展示对应通信域和算子的通信信息
    test('visualized communication time chart', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const communicationTime = communicationFrame.locator('#main');
        await communicationTime.scrollIntoViewIfNeeded();
        await page.mouse.move(0, 0);
        await expect(communicationTime).toHaveScreenshot('visualized-communication-time.png');
    });

    // 【case】专家建议
    test('communication advice', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const advice = communicationFrame.getByTestId('communicationAdvice');
        await advice.scrollIntoViewIfNeeded();
        await page.mouse.move(0, 0);
        await expect(advice).toHaveScreenshot('communication-advice.png');
    });

    // 【case】通信时长数据分析：展示对应通信域和算子 表头排序
    test('table header sort', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const tableLocator = communicationFrame.getByTestId('dataAnalysisTable').locator('.ant-table-container > .ant-table-content > table');
        const dataAnalysisTable = new TableHelpers(page, tableLocator, communicationFrame);
        await tableLocator.scrollIntoViewIfNeeded();
        await dataAnalysisTable.sortTableHead('Elapse Time(ms)');
        await expect(tableLocator).toHaveScreenshot('data-analysis-table-sort.png');
    });

    // 【case】通信时长数据分析：展示对应通信域和算子 点击icon / button展开子表格
    test('expand sub table', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const tableLocator = communicationFrame.getByTestId('dataAnalysisTable').locator('.ant-table-container > .ant-table-content > table').first();
        const dataAnalysisTable = new TableHelpers(page, tableLocator, communicationFrame);
        // 定位并点击展开icon
        const expandIcon = (await dataAnalysisTable.getCell(1, 1)).locator('div div');
        await expandIcon.click();
        // 定位第一行数据的子表格 验证子表格可见
        const expandTable = communicationFrame.getByTestId('dataAnalysisTable').locator('.ant-table-container > .ant-table-content > table').nth(1);
        await expect(expandTable).toBeVisible();
        // 再次点击icon 收起子表格
        await expandIcon.click();
        // 定位并点击查看详情按钮 再次验证子表格是否可见
        const expandButton = await (await dataAnalysisTable.getCell(1, 13)).locator('button');
        await expandButton.click();
        await expect(expandTable).toBeVisible();
    });

    // 【case】通信时长数据分析：展示对应通信域和算子 通信算子详情子表格排序和分页
    test('communication operator details sub table', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        // 切换到通信耗时分析并点击icon展开子表格
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const tableLocator = communicationFrame.getByTestId('dataAnalysisTable').locator('.ant-table-container > .ant-table-content > table').first();
        const dataAnalysisTable = new TableHelpers(page, tableLocator, communicationFrame);
        const expandIcon = (await dataAnalysisTable.getCell(1, 1)).locator('div div');
        await expandIcon.click();
        // 定位到子表格元素
        const expandTableLocator = communicationFrame.getByTestId('dataAnalysisTable').locator('.ant-table-container > .ant-table-content > table').nth(1);
        const expandTable = new TableHelpers(page, expandTableLocator, communicationFrame);
        // 表格滚动到可视区域并点击表头排序
        await expandTable.sortTableHead('Start Time(ms)');
        await expandTableLocator.scrollIntoViewIfNeeded();
        await expect(expandTableLocator).toHaveScreenshot('data-analysis-subtable-sort.png');
        // 定位子表格分页器组件
        const pagination = communicationFrame.locator('.ant-pagination.ant-pagination-mini.ant-table-pagination.ant-table-pagination-left').first();
        // 断言数据总数
        const totalTextDiv = pagination.locator('li').first().locator('div');
        expect(totalTextDiv).toHaveText('Total 59 items');
        // 点击分页按钮
        const pageLink = pagination.locator('.ant-pagination-item.ant-pagination-item-3 a'); // 第三页
        const prevPage = pagination.locator('.ant-pagination-prev button');
        const nextPage = pagination.locator('.ant-pagination-next button');
        await pageLink.click();
        await prevPage.click();
        await nextPage.click();
        expect(await expandTable.getCell(1, 2)).toHaveText('331.2512');
        // 验证分页器输入框
        const paginationInput = pagination.locator('.ant-pagination-options .ant-pagination-options-quick-jumper input');
        await paginationInput.focus();
        await paginationInput.fill('1');
        await paginationInput.press('Enter');
        expect(await expandTable.getCell(1, 2)).toHaveText('118.5898');
    });

    // 【case】点击“带宽分析”列的查看更多可以跳转至所选rank的对应算子带宽分析页
    test('jump to the bandwidth analysis page', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis, toOperatorpage } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        await toOperatorpage(page, communicationFrame);
        const operatorPage = communicationFrame.getByTestId('operators');
        await operatorPage.waitFor({
            state: 'visible',
        });
        const operatorPageHeaderText = operatorPage.getByTestId('operatorRankId');
        expect(operatorPageHeaderText).toHaveText('Total Op Info(RankId 6)');
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
