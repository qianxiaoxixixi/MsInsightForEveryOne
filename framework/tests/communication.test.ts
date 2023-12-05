import { test, expect, type Page } from '@playwright/test';
import {selectFolder} from './baseOperation';
test.describe('comminucation',() => {
    test.describe.configure({ mode: 'serial' });
    let page: Page;
    test.beforeAll('Open Page', async ({ browser, baseURL }) => {
        page = await browser.newPage({viewport:{width:1700,height:1100}});
        await page.goto(baseURL);
    });

    test.afterAll(async () => {
        await page.close();
    });

    test('testCommunicationTabWithNoData', async () => {
        expect(await page.getByText('Communication').count()).toBe(0);
    });

    test('testCommunicationTabWithData', async () => {
        await selectFolder({page});
        await page.getByText('Communication').click();
        await page.waitForTimeout(1500);
        expect(page).toHaveScreenshot('communication.png', { maxDiffPixels: 800 });
    });

    test('testDurationTablePageChange', async () => {
        await page.locator('li.ant-pagination-item-2').count();
    });
});