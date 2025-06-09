/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import { runInAction } from 'mobx';
import type { Session } from '../entity/session';
import type { ThreadMetaData } from '../entity/data';

const isEventMenuVisible = (session: Session): boolean => {
    // 必须只选中一个才能显示“跳转数据窗口事件视图”菜单项
    if (session.selectedUnits.length !== 1) {
        return false;
    }
    const selectUnit = session.selectedUnits[0];
    if (selectUnit === undefined || session.isSimulation) {
        return false;
    }
    if (['Empty', 'Card', 'Counter', 'Root'].includes(selectUnit.name)) {
        return false;
    }
    if (selectUnit.children) {
        for (const child of selectUnit.children) {
            if (child.name === 'Counter') {
                return false;
            }
        }
    }
    if ((selectUnit.metadata as ThreadMetaData).threadName?.includes('Plane')) {
        return false;
    }
    return true;
};

export const actionShowInEventsView = register({
    name: 'showInEventsView',
    label: 'timeline:contextMenu.Show in Events View',
    visible: (session) => isEventMenuVisible(session),
    perform: (session): void => {
        runInAction(() => {
            session.showEvent = !session.showEvent;
            session.eventUnits = session.selectedUnits;
        });
    },
});
