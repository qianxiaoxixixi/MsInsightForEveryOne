import { test, expect, type Page } from '@playwright/test';
import {selectFolder} from './baseOperation';
test.describe('timeline',() => {
    test.describe.configure({ mode: 'serial' });
    let page: Page;
    test.beforeAll('Open Page', async ({ browser, baseURL }) => {
        page = await browser.newPage({ viewport: { width: 1700,height: 1100 }});
        await page.goto(baseURL);
    });

    test.afterAll(async () => {
        await page.close();
    });

    // 测试收藏泳道第一层展开功能
    test('testSwimlaneTopLayerExpand', async () => {
        // 导入数据
        const path = 'D:\\ascendInsight\\16ka_gpt3\\master47_1993648_20230919172551_ascend_pt';
        await selectFolder({page, path});
        await page.waitForTimeout(5000);
        // 选择Timeline页签
        await page.getByRole('menuitem', { name: 'Timeline View' })?.click();
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('svg.insight-unit-fold').first().click();
        await expect(await frame.getByText('Python (1993648)')).toBeVisible();
        // 复原
        await frame.locator('svg.insight-unit-expanded').first().click();
        await expect(await page.screenshot({fullPage: true})).toMatchSnapshot('resetting.png');
    });

    // 测试收藏泳道功能
    test('testSwimlaneTopLayerPinnedUnpinned', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('svg.insight-unit-fold').first().click();
        await frame.locator('div.unit-info.css-8avfj3').first().hover();
        await frame.locator('div.unit-info.css-8avfj3').locator('button.ant-btn.ant-btn-default.ant-btn-icon-only.css-b5a2p1').first().click();
        await expect(page).toHaveScreenshot('top_layer_pinned.png', { maxDiffPixels: 800 });
        // 取消收藏
        await frame.locator('div.unit-info.css-8avfj3').locator('button.ant-btn.ant-btn-default.ant-btn-icon-only.css-b5a2p1').first().click();
        await expect(page).toHaveScreenshot('top_layer_unpinned.png', { maxDiffPixels: 800 });
        await frame.locator('svg.insight-unit-expanded').first().click();
        await expect(page).toHaveScreenshot('resetting.png', { maxDiffPixels: 800 });
    });

    // 测试泳道展开到第三层
    test('testSwimlaneSecondLayerExpand', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('svg.insight-unit-fold').first().click();
        await frame.locator('svg.insight-unit-fold').nth(2).click();
        await frame.waitForTimeout(1500);
        // 预期第二层数据呈现
        await expect(await frame.getByText('Stream 12')).toBeVisible();
        await frame.locator('svg.insight-unit-expanded').nth(1).click();
        await frame.waitForTimeout(1500);
        await frame.locator('svg.insight-unit-expanded').first().click();
        await expect(page).toHaveScreenshot('resetting.png', { maxDiffPixels: 800 });
    });

    // 测试泳道复位功能
    test('testSwimlaneThirdLayerReset', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('svg.insight-unit-fold').first().click();
        await frame.locator('svg.insight-unit-fold').nth(0).click();
        await frame.waitForTimeout(1500);
        await frame.locator('svg.css-1x2odwv').click();
        await frame.waitForTimeout(1500);
        await expect(await page.screenshot({fullPage: true})).toMatchSnapshot('third_layer_reset.png');
        await frame.locator('svg.insight-unit-expanded').nth(1).click();
        await frame.waitForTimeout(1500);
        await frame.locator('svg.insight-unit-expanded').first().click();
        await expect(page).toHaveScreenshot('resetting.png', { maxDiffPixels: 800 });
    });

    // 测试数据块详情展示
    test('testSwimlaneThirdLayerSingleSelect', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('svg.insight-unit-fold').first().click();
        await frame.locator('svg.insight-unit-fold').nth(0).click();
        await frame.waitForTimeout(1500);
        await frame.locator('svg.css-1x2odwv').click();
        await frame.waitForTimeout(1500);
        // 点击要展示的数据块
        await page.locator('body').click({
            position: { x: 690, y: 160 },
        });
        await frame.waitForTimeout(1500);
        // 展示详情数据
        await expect(await frame.getByText('Self Time')).toBeVisible();
        await frame.locator('svg.insight-unit-expanded').nth(1).click();
        await frame.waitForTimeout(1500);
        await frame.locator('svg.insight-unit-expanded').first().click();
        await frame.locator('span.anticon.anticon-caret-right.caret').click();
    });

    // 测试顶层泳道数据收起功能
    test('testSwimlaneTopLayerFold', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('svg.insight-unit-fold').first().click();
        await frame.locator('svg.insight-unit-expanded').first().click();
        await expect(await frame.getByText('Python (1993648)')).not.toBeVisible();
    });

    // 测试泳道offset功能
    test('testSwimlaneThirdLayerOffset', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        // 展开泳道
        await frame.locator('svg.insight-unit-fold').first().click();
        await frame.locator('svg.insight-unit-fold').nth(2).click();
        await frame.waitForTimeout(1500);
        // reset展示数据
        await frame.locator('svg.css-1x2odwv').click();
        await frame.waitForTimeout(1500);
        await frame.getByText('Offset').first().click();
        await frame.waitForTimeout(1500);
        // 输入正数泳道数据向左偏移
        await frame.locator('.css-4lza26').locator('input').fill('1000000');
        await frame.waitForTimeout(1500);
        await page.keyboard.down("Enter");
        await expect(page).toHaveScreenshot('layer_offset_left.png', { maxDiffPixels: 900 });
        // 输入0泳道数据向右偏移
        await frame.locator('.css-4lza26').locator('input').fill('0');
        await frame.waitForTimeout(1500);
        await page.keyboard.down("Enter");
        await expect(page).toHaveScreenshot('layer_offset_right.png', { maxDiffPixels: 900 });
        // 复位页面
        await frame.locator('svg.insight-unit-expanded').nth(1).click();
        await frame.locator('svg.insight-unit-expanded').first().click();
    });

    // 测试systemView表格初始化
    test('testSystemViewExist', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.locator('span.anticon.anticon-caret-left.caret').click();
        await frame.getByText('StatsSystemView').click();
        await expect(await frame.getByText('Python API Summary')).toBeVisible();
    });

    // 测试表格切换
    test('testSystemViewChangeTag', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.getByText('CANN API Summary').click();
        await expect(page).toHaveScreenshot('system_view_static_table_cann.png', { maxDiffPixels: 1500 });
        await frame.getByText('Ascend HardWare Task Summary').click();
        await expect(page).toHaveScreenshot('system_view_static_table_task.png', { maxDiffPixels: 1500 });
        await frame.getByText('HCCL Summary').click();
        await expect(page).toHaveScreenshot('system_view_static_table_hccl.png', { maxDiffPixels: 1500 });
        await frame.getByText('Overlap Analysis').click();
        await expect(page).toHaveScreenshot('system_view_static_table_overlap.png', { maxDiffPixels: 1500 });
        await frame.getByText('Kernel Details').click();
        await expect(page).toHaveScreenshot('system_view_static_table_kernel.png', { maxDiffPixels: 1500 });
    });

    // 测试统计表格按照Num Calls进行排序
    test('testSystemViewStaticSort', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.getByText('Python API Summary').click();
        // Num Calls升序
        await frame.locator('th').filter({ hasText: 'Num Calls' }).click();
        await expect(page).toHaveScreenshot('system_view_table_aes.png', { maxDiffPixels: 1500 });
        // Num Calls降序
        await frame.locator('th').filter({ hasText: 'Num Calls' }).click();
        await expect(page).toHaveScreenshot('system_view_table_des.png', { maxDiffPixels: 1500 });
        // 恢复
        await frame.locator('th').filter({ hasText: 'Num Calls' }).click();
        await expect(page).toHaveScreenshot('system_view_table_reset.png', { maxDiffPixels: 1500 });
    });

    // 测试kernel detail表格按照Block Dim进行排序
    test('testSystemViewKernelSort', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.getByText('Kernel Details').click();
        // Block Dim升序
        await frame.locator('th').filter({ hasText: 'Block Dim' }).click();
        await expect(page).toHaveScreenshot('system_view_table_kernel_aes.png', { maxDiffPixels: 1500 });
        // Block Dim降序
        await frame.locator('th').filter({ hasText: 'Block Dim' }).click();
        await expect(page).toHaveScreenshot('system_view_table_kernel_des.png', { maxDiffPixels: 1500 });
        // 恢复
        await frame.locator('th').filter({ hasText: 'Block Dim' }).click();
        await expect(page).toHaveScreenshot('system_view_table_kernel_reset.png', { maxDiffPixels: 1500 });
    });

    // 测试统计数据表切换数据和切换页面显示个数
    test('testSystemViewStaticTablePageChange', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.getByText('Python API Summary').click();
        // 下一页
        await frame.locator('li.ant-pagination-item-2').click();
        await frame.waitForTimeout(500);
        await expect(page).toHaveScreenshot('system_view_table_page_to_2.png', { maxDiffPixels: 1500 });
        // 切换每页记录个数
        await frame.getByText('10 / page').click();
        await frame.getByText('20 / page').click();
        await frame.waitForTimeout(1000);
        await frame.locator('div.ant-tabs-content.ant-tabs-content-top').click();
        await page.mouse.wheel(0, 1000);
        await expect(page).toHaveScreenshot('system_view_table_page_20.png', { maxDiffPixels: 1500 });
    });

    // 测试kernel detail表格切换数据和切换页面显示个数
    test('testSystemViewKernelTablePageChange', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.getByText('Kernel Details').click();
        // 下一页
        await frame.locator('li.ant-pagination-item-2').click();
        await frame.waitForTimeout(500);
        await expect(page).toHaveScreenshot('system_view_kernel_table_page_to_2.png', { maxDiffPixels: 1500 });
        // 切换每页记录个数
        await frame.getByText('10 / page').click();
        await frame.getByText('20 / page').click();
        await frame.waitForTimeout(1000);
        await frame.locator('div.ant-tabs-content.ant-tabs-content-top').click();
        await page.mouse.wheel(0, 1000);
        await expect(page).toHaveScreenshot('system_view_kernel_table_page_20.png', { maxDiffPixels: 1500 });
    });

    // 测试kernel detail表格数据和timeline联动
    test('testSystemViewKernelLinkTimeline', async () => {
        const frame = page.frame({ url: /.Timeline.*/  });
        await frame.getByText('Kernel Details').click();
        // 点击click
        await frame.getByRole('button', { name: 'click' }).first().click();
        await expect(page).toHaveScreenshot('system_view_kernel_link_timeline.png', { maxDiffPixels: 1500 });
        // 还原成初始导入数据的样子
        await frame.locator('svg.insight-unit-expanded').nth(1).click();
        await frame.locator('svg.insight-unit-expanded').first().click();
        await frame.locator('span.anticon.anticon-caret-right.caret').click();
        await expect(page).toHaveScreenshot('resetting.png', { maxDiffPixels: 1500 });
    });
});
