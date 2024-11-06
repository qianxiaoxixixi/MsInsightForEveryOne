/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { test, expect } from '@playwright/test';
import { FrameworkPage } from './page-object';
import { clearAllData, importData } from './utils';

test.describe('Framework', () => {
    test.beforeEach(async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        await frameworkPage.goto();
    });

    // 导入项目
    test('import data', async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        await frameworkPage.goto();
        await importData(page);
    });

    // 清空项目
    test('clear all data', async ({ page }) => {
        await importData(page);
        await clearAllData(page);
    });

    // 切换主题
    test('toggle theme', async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        await frameworkPage.switchThemeBtn.click();
    });

    // 切换中英文
    test('toggle language', async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        await frameworkPage.switchLanguageBtn.click();
    });

    // 查看帮助信息
    test('view help info', async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        const { helpInfoDialog } = frameworkPage;
        await frameworkPage.helpInfoBtn.click();
        await expect(helpInfoDialog).toBeVisible();
    });
});
