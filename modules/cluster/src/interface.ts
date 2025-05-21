/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
*/
import type { NotificationHandler } from './connection/defs';
import {
    removeRemoteHandler,
    setTheme,
    updateSessionHandler,
    locateCommunication,
    switchLanguageHandler,
    updateCommunicatorDataHandler,
    baselineToggleHandler,
    switchDirectoryHandler,
    frameLoadedHandler,
    updateClusterPageInfoHandler,
} from './connection/handler';

interface InsightInterface<Request extends Record<string, unknown>, Response extends Record<string, unknown>> {
    request: Request;
    response: Response;
};

export interface InterfaceDefs {
    'chart/cpu': InsightInterface<{ chartId: number }, { data: Array<{ ts: number; value: number }>}>;
};

export const NOTIFICATION_HANDLERS: Record<string, NotificationHandler<any>> = {
    'frame/loaded': frameLoadedHandler,
    'remote/remove': removeRemoteHandler,
    'remote/reset': removeRemoteHandler,
    'module.reset': removeRemoteHandler,
    setTheme,
    updateClusterPageInfo: updateClusterPageInfoHandler,
    updateSession: updateSessionHandler,
    locateCommunication,
    switchLanguage: switchLanguageHandler,
    updateCommunicatorData: updateCommunicatorDataHandler,
    clusterBaselineToggle: baselineToggleHandler,
    switchDirectory: switchDirectoryHandler,
};
