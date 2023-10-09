import { test, expect } from '@playwright/test'

test('basic layout test', async ({ page, baseURL }) => {
  await page.goto(baseURL);
  // check basic layout by snapshot
  expect(await page.screenshot({fullPage: true})).toMatchSnapshot('mainPage.png');
  await page.locator('.el-aside > .header > .icon-button').click();
  
  expect(await page.getByLabel('File Explorer').screenshot()).toMatchSnapshot('fileExplorer.png');
});
