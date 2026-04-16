/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 */

import { expect, test } from '@playwright/test';
import { FrameworkPage } from '@/page-object';
import { clearAllData } from '@/utils';


test.describe('Framework', () => {
    test.beforeEach(async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        await frameworkPage.goto();
    });

    // 查看帮助信息
    test('view help info', async ({ page }) => {
        const frameworkPage = new FrameworkPage(page);
        const { helpInfoDialog, shortcutsDialog } = frameworkPage;
        await frameworkPage.helpInfoBtn.click();
        await page.getByText('Keyboard shortcuts').click();
        await expect(shortcutsDialog).toBeVisible();
        await page.getByLabel('Close', { exact: true }).click();

        await frameworkPage.helpInfoBtn.click();
        await page.getByText('About').click();
        await expect(helpInfoDialog).toBeVisible();
    });
});
