/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import type { Action } from './types';

const _actions: Action[] = [];
export const actions: readonly Action[] = _actions;

export const register = <T extends Action>(action: T): T => {
    _actions.push(action);
    return action;
};
