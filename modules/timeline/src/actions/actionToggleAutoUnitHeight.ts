/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import type { Session } from '../entity/session';
import { runInAction } from 'mobx';

const toggleAutoUnitHeight = (session: Session): void => {
    runInAction(() => {
        session.autoAdjustUnitHeight = !session.autoAdjustUnitHeight;
        session.renderTrigger = !session.renderTrigger;
    });
};

export const actionEnableAutoUnitHeight = register({
    name: 'enableAutoUnitHeight',
    label: 'timeline:contextMenu.Enable auto unit height',
    visible: (session) => !session.autoAdjustUnitHeight,
    perform: (session): void => {
        toggleAutoUnitHeight(session);
    },
});

export const actionDisableAutoUnitHeight = register({
    name: 'disableAutoUnitHeight',
    label: 'timeline:contextMenu.Disable auto unit height',
    visible: (session) => session.autoAdjustUnitHeight,
    perform: (session): void => {
        toggleAutoUnitHeight(session);
    },
});
