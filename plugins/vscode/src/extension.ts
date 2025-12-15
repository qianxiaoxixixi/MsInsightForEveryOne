/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
'use strict';

import * as vscode from 'vscode';
import type { RegisterWebview } from './webview/RegisterWebview';
import { webviewManager } from './webview/WebviewManager';
export function activate(context: vscode.ExtensionContext): void {
	context.subscriptions.push(
		vscode.commands.registerCommand('mindstudio-insight.start', () => {
			const webview = webviewManager.createWebview('insight', 'insight', context) as RegisterWebview;
			webview.previewUIPage();
		})
	);
}