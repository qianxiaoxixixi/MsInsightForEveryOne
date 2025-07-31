/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { NotificationHandler } from '@/connection/defs';
import { setThemeHandler, switchLanguageHandler } from '@/connection/handler/settingsHandler';

export const NOTIFICATION_HANDLERS: Record<string, NotificationHandler<any>> = {
    setTheme: setThemeHandler,
    switchLanguage: switchLanguageHandler,
};
