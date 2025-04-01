/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import type { Session } from '../entity/session';
import { runInAction } from 'mobx';
import type { InsightUnit } from '../entity/insight';

const setUpUintOffset = (session: Session, insightUint: InsightUnit, offsetValue: number): void => {
    session.setTimestampOffsetByUnit(insightUint, offsetValue, false);
};

const clearOrRecoverCardDefaultOffset = (session: Session): void => {
    runInAction(() => {
        session.units.forEach((insightUint) => {
            setUpUintOffset(session, insightUint, 0);
        });
        session.updateEndTimeAll();
        session.setDomainWithoutHistory({ domainStart: 0, domainEnd: session.endTimeAll ?? session.domain.defaultDuration });
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
