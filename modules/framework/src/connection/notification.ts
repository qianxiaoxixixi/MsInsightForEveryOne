/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import {
    deleteCardHandler,
    getParseStatusHandler,
    getThemeHandler,
    switchModuleHandler,
    updateSessionHandler,
    getLanguageHandler,
    openImportDialogHandler,
    clusterCompletedHandler,
    clusterDurationCompletedHandler,
} from './notificationHandler';
import { sendWakeupPlugin } from '@/connection/sendNotification';

export interface NotificationMessage {
    data: {
        body: {
            [p: string]: unknown;
            from?: string;
            broadcast?: boolean;
            switchTo?: string;
            toModuleEvent?: string;
        };
        event: string;
        module: string;
        from?: number;
        modules?: string[];
        port?: number;
    };
    isTrusted?: boolean;
}

type ListenerCallback = (res: NotificationMessage) => void;

const PARSE_CLUSTER_COMPLETED = 'frame:parseClusterCompleted';
const PARSE_CLUSTER_STEP2_COMPLETED = 'frame:parseClusterDurationCompleted';

export const listenerMap: Record<string, ListenerCallback> = {
    updateSession: updateSessionHandler,
    getParseStatus: getParseStatusHandler,
    getTheme: getThemeHandler,
    deleteCard: deleteCardHandler,
    switchModule: switchModuleHandler,
    getLanguage: getLanguageHandler,
    pluginMounted: sendWakeupPlugin,
    openImportDialog: openImportDialogHandler,
    [PARSE_CLUSTER_COMPLETED]: clusterCompletedHandler,
    [PARSE_CLUSTER_STEP2_COMPLETED]: clusterDurationCompletedHandler,
};
