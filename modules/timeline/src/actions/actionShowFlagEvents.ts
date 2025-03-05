/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import type { Session } from '../entity/session';
import { runInAction } from 'mobx';

const hideOrShowFlagEvents = (session: Session): void => {
    if (!session.isSimulation) {
        return;
    }
    runInAction(() => {
        session.areFlagEventsHidden = !session.areFlagEventsHidden;
        if (session.areFlagEventsHidden) {
            session.singleLinkLine = {};
            session.linkLines = {};
            session.renderTrigger = !session.renderTrigger;
        }
    });
};

export const actionShowFlagEvents = register({
    name: 'showFlagEvents',
    label: 'timeline:contextMenu.Show flag events',
    visible: (session) => session.isSimulation && session.areFlagEventsHidden,
    perform: (session): void => {
        hideOrShowFlagEvents(session);
    },
});

export const actionHideFlagEvents = register({
    name: 'hideFlagEvents',
    label: 'timeline:contextMenu.Hide flag events',
    visible: (session) => session.isSimulation && !session.areFlagEventsHidden,
    perform: (session): void => {
        hideOrShowFlagEvents(session);
    },
});
