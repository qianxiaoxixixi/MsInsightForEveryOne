/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import type { NotificationHandler } from './connection/defs';
import {
    parseMemoryCompletedHandler,
    removeRemoteHandler,
    setTheme,
    updateSessionHandler,
    deleteCardHandler,
    allSuccessHandler,
    switchLanguageHandler,
    switchDirectoryHandler,
    updateRangeFlagListHandler,
    updateTimelineOffsetHandler,
    moduleActiveHandler,
} from './connection/handler';

export const NOTIFICATION_HANDLERS: Record<string, NotificationHandler> = {
    'parse/memoryCompleted': parseMemoryCompletedHandler,
    allPagesSuccess: allSuccessHandler,
    'remote/remove': removeRemoteHandler,
    'remote/reset': removeRemoteHandler,
    'module.reset': removeRemoteHandler,
    updateSession: updateSessionHandler,
    setTheme,
    deleteCard: deleteCardHandler,
    switchLanguage: switchLanguageHandler,
    switchDirectory: switchDirectoryHandler,
    updateRangeFlagList: updateRangeFlagListHandler,
    updateTimelineOffset: updateTimelineOffsetHandler,
    moduleActive: moduleActiveHandler,
};
