import { test } from '@playwright/test';
import {selectFolder} from './baseOperation';
const path= require('path');

test('select folder', async ({ page, baseURL }) => {
    await page.goto(baseURL);
    await selectFolder({ page, baseURL });
});
test('test interface', async ({ page, baseURL }) => {
        await page.goto(baseURL);
        await selectFolder({ page, baseURL });
        const res = await page.evaluate(async() => {
            // 从0开始
            const res = await (window[2] as any).requestData(
                'operator/category',
                {
                    fileId: '15',
                    rankId: '15',
                    group: 'Operator Type',
                    topK: 15,
                    custom: 0,
                },
            );
            return res;
        });
    },
);