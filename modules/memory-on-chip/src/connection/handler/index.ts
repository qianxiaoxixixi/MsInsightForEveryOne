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
import { NotificationHandler } from '@/connection/defs';
import { setThemeHandler, switchLanguageHandler } from '@/connection/handler/settingsHandler';
import { parseCompletedHandler, removeRemoteHandler, parseFailHandler } from '@/connection/handler/dataHandler';

export const NOTIFICATION_HANDLERS: Record<string, NotificationHandler<any>> = {
    setTheme: setThemeHandler,
    switchLanguage: switchLanguageHandler,
    'parse/tritonCompleted': parseCompletedHandler,
    'parse/fail': parseFailHandler,
    'remote/remove': removeRemoteHandler,
    'remote/reset': removeRemoteHandler,
};
