import { expect } from '@playwright/test'

export async function selectFolder({ page, baseURL}: { baseURL: string; page: any;path?: string }) {
    await page.goto(baseURL);
    await page.locator('.el-aside > .header > .icon-button').click();
    await page.getByText('D:\\').click();
    await page.locator('span[name=Dworkspace]').click();
    await page.locator('span[name=Dworkspacedata]').click();
    await page.locator('span[name=Dworkspacedata16ka_gpt3]').click();
    await page.getByText('Confirm').click();
    await page.waitForTimeout(100);
    expect(page).toHaveScreenshot('selectFolder.png', { maxDiffPixels: 800 });
    await page.waitForTimeout(15000);
    expect(await page.screenshot({ fullPage: true })).toMatchSnapshot('parseSuccess.png');
}

export async function selectFile({ page, baseURL }: { baseURL: string; page: any;path?: string }) {
    await page.goto(baseURL);
    await page.locator('.el-aside > .header > .icon-button').click();
    await page.getByText('D:\\').click();
    await page.locator('span[name=Dworkspace]').click();
    await page.locator('span[name=Dworkspacedata]').click();
    await page.locator('span[name=Dworkspacedata16ka_gpt3]').click();
    await page.locator('span[name=Dworkspacedata16ka_gpt3localhost.localdomain_1408366_20230919172258_ascend_pt]').click();
    await page.locator('span[name=Dworkspacedata16ka_gpt3localhost.localdomain_1408366_20230919172258_ascend_ptASCEND_PROFILER_OUTPUT]').click();
    await page.getByText('trace_view.json').click();
    await page.getByText('Confirm').click();
    await page.waitForTimeout(500);
}