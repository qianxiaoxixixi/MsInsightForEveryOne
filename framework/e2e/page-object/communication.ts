/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import type { FrameLocator, Page, Locator } from '@playwright/test';
import { FrameworkPage } from './framework';

export class CommunicationPage {
    readonly page: Page;
    readonly communicationFrame: FrameLocator;
    readonly stepSelector: Locator;
    readonly communicationGroupSelector: Locator;
    readonly operatorNameSelector: Locator;
    readonly durationAnalysisRadio: Locator;
    readonly communicationMatrixRadio: Locator;
    readonly communicationMatrixTypeSelector: Locator;

    constructor(page: Page) {
        this.page = page;
        this.communicationFrame = page.frameLocator('#Communication');
        this.stepSelector = this.communicationFrame.locator('#step-select'); // 迭代id
        this.communicationGroupSelector = this.communicationFrame.locator('#communicationGroup-select'); // 通信域
        this.operatorNameSelector = this.communicationFrame.locator('#operatorName-select'); // 算子名称
        this.communicationMatrixRadio = this.communicationFrame.locator('input[type="radio"][value="CommunicationMatrix"]'); // 通信矩阵
        this.durationAnalysisRadio = this.communicationFrame.locator('input[type="radio"][value="CommunicationDurationAnalysis"]'); // 通信耗时分析
        this.communicationMatrixTypeSelector = this.communicationFrame.locator('#communicationMatrixType-select'); // 通信矩阵类型
    }

    async goto(): Promise<void> {
        const frameworkPage = new FrameworkPage(this.page);
        await frameworkPage.clickTab('communication');
    }
}
