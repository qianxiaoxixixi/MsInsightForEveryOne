/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import { Browser } from './Browser';
import { IntellijPlatform } from './IntellijPlatform';
import { VsCodePlatform } from './VsCodePlatform';
import type { BasePlatform } from './BasePlatform';

declare function acquireVsCodeApi(): any;

let platform: BasePlatform = new Browser();
if (typeof acquireVsCodeApi === 'function') {
    platform = new VsCodePlatform();
    platform.sendMessage = acquireVsCodeApi().postMessage;
}

if (typeof window.cefQuery === 'function') {
    platform = new IntellijPlatform();
}

export const MESSAGE_SENDER: BasePlatform = platform;