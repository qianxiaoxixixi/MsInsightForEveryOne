/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import React from 'react';
import type { Action, ActionName } from './types';
import { Session } from '../entity/session';
import type { InteractorMouseState } from '../components/charts/ChartInteractor/ChartInteractor';

export class ActionManager {
    actions = {} as Record<ActionName, Action>;
    session: Session;
    pressedKeys = new Set<string>();

    constructor(session: Session) {
        this.session = session;
    }

    registerAction(action: Action): void {
        this.actions[action.name] = action;
    }

    registerAll(actions: readonly Action[]): void {
        actions.forEach((action) => this.registerAction(action));
    }

    handleKeyDown(
        event: React.KeyboardEvent | KeyboardEvent,
        interactorMouseState: InteractorMouseState,
        xScale?: (x: number) => number,
    ): void {
        const activeElement = document.activeElement;
        const isInputType = activeElement instanceof HTMLInputElement ||
            activeElement instanceof HTMLTextAreaElement ||
            (activeElement instanceof HTMLElement && activeElement.isContentEditable);
        // 输入框内不执行快捷键
        if (isInputType) {
            return;
        }

        const data = Object.values(this.actions)
            .filter((action) =>
                action?.keyTest && action.keyTest(event),
            );

        if (data.length === 0) {
            return;
        }

        const action = data[0];
        const key = event.key.toLowerCase();

        // 每次按键只执行一次
        if (action.once) {
            if (this.pressedKeys.has(key)) {
                return;
            } else {
                this.pressedKeys.add(key);
            }
        }

        event.preventDefault();
        event.stopPropagation();
        action.perform(this.session, interactorMouseState, xScale);
    }

    handleKeyUp(event: React.KeyboardEvent | KeyboardEvent): void {
        this.pressedKeys.clear();
    }
}
