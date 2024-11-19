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

    // 数据展示配置【case】
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

    // 通信矩阵配置【case】
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
        await page.mouse.move(0, 0);
        await expect(matrixChart).toHaveScreenshot('communication-matrix.png');
    });

    // 通信耗时分析 hccl
    test('select duration analysis hccl', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        await page.mouse.move(0, 0);
        await expect(communicationFrame.locator('#hccl')).toHaveScreenshot('communication-hccl.png');
    });

    // 通信耗时分析 通信时长
    test('visualized communication time', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const communicationTime = communicationFrame.locator('#main');
        await communicationTime.scrollIntoViewIfNeeded();
        await page.mouse.move(0, 0);
        await expect(communicationTime).toHaveScreenshot('visualized-communication-time.png');
    });

    // 通信耗时分析 专家建议
    test('communication advice', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const advice = communicationFrame.getByTestId('communicationAdvice');
        await advice.scrollIntoViewIfNeeded();
        await page.mouse.move(0, 0);
        await expect(advice).toHaveScreenshot('communication-advice.png');
    });

    // 表格截图对比
    test('table screen shot', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const dataAnalysisTable = communicationFrame.getByTestId('dataAnalysisTable');
        await dataAnalysisTable.scrollIntoViewIfNeeded();
        await page.mouse.move(0, 0);
        await expect(dataAnalysisTable).toHaveScreenshot('data-analysis-table.png');
    });

    // 表格行数
    test('table rows', async ({ page, communicationPage }) => {
        const { communicationMatrixRadio, durationAnalysisRadio, communicationFrame, switchDurationAnalysis } = communicationPage;
        await switchDurationAnalysis(communicationMatrixRadio, durationAnalysisRadio);
        const tableLocator = communicationFrame.getByTestId('dataAnalysisTable').locator('.ant-table-container > .ant-table-content > table');
        const dataAnalysisTable = new TableHelpers(page, tableLocator, communicationFrame);
        const rows = await dataAnalysisTable.getTableRows();
        expect(rows).toEqual(8);
    });

    test.afterEach(async ({ page }) => {
        await clearAllData(page);
    });
});
