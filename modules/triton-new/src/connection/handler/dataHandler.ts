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
import { rootStore } from '@/stores';
import { errorCenter, ErrorCode, WsError } from '@insight/lib';
import type { NotificationHandler } from '@/connection/defs';

export const parseCompletedHandler: NotificationHandler = (): void => {
    // When framework notifies that a new file is parsed, we fetch new data
    rootStore.tritonStore.clearData();
    rootStore.tritonStore.fetchBasicInfo();
};

export const removeRemoteHandler: NotificationHandler = (): void => {
    // When framework notifies to remove current view/file, we clear data
    rootStore.tritonStore.clearData();
};

export const parseFailHandler: NotificationHandler = (data): void => {
    // Handle framework parsing errors and show globally
    errorCenter.handleError(new WsError(ErrorCode.PARSE_FAIL, data?.error as string));
};
