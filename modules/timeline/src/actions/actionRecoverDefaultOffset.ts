/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import type { Session } from '../entity/session';
import { runInAction } from 'mobx';
import type { InsightUnit } from '../entity/insight';
import { getTimeOffsetKey } from '../insight/units/utils';
import type { CardMetaData, ThreadTraceRequest } from '../entity/data';

const setUpUintOffset = (session: Session, insightUint: InsightUnit, offsetValue: number): void => {
    const prevObj = session.unitsConfig.offsetConfig.timestampOffset;
    if (insightUint.children !== undefined && insightUint.children.length > 0) {
        for (const item of insightUint.children) {
            const key = getTimeOffsetKey(session, item.metadata as ThreadTraceRequest);
            session.unitsConfig.offsetConfig.timestampOffset[key] = offsetValue;
        }
    }
    session.unitsConfig.offsetConfig.timestampOffset = {
        ...prevObj,
        [(insightUint.metadata as CardMetaData).cardId]: (offsetValue),
    };
};

const clearOrRecoverCardDefaultOffset = (session: Session): void => {
    runInAction(() => {
        session.units.forEach((insightUint) => {
            const offsetValue = insightUint.alignStartTimestamp as number;
            setUpUintOffset(session, insightUint, offsetValue);
        });
        session.benchMarkData = undefined;
        session.alignSliceData = [];
    });
};

export const actionRecoverDefaultOffset = register({
    name: 'recoverDefaultOffset',
    label: 'timeline:contextMenu.Recover cards default offset',
    perform: (session): void => {
        clearOrRecoverCardDefaultOffset(session);
    },
});
