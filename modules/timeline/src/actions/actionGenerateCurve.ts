/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { register } from './register';
import { Session } from '../entity/session';
import { createCurve } from '../api/request';
import connector from '../connection';

async function findLineChart(session: Session, type: string): Promise<void> {
    if (!session.selectedData) {
        return;
    }
    const { name, cardId, processId, threadId } = session.selectedData;
    const params = {
        fileId: cardId as string,
        pid: processId,
        tid: threadId,
        x: name,
        type,
    };
    const res = await createCurve(params);
    connector.send({
        event: 'switchModule',
        body: {
            switchTo: 'statistic',
            toModuleEvent: 'locateGroup',
            params: {
                fileId: cardId as string,
                group: res.curveName,
            },
        },
    });
}

function checkCardIsIE(session: Session): boolean {
    const { isIE, isCluster, selectedData, units } = session;
    const cluster = units.find(unit => unit.metadata?.cardId === selectedData?.cardId)?.metadata?.cluster;
    return Boolean(isIE && selectedData && (!isCluster || (isCluster && !cluster)));
}

export const actionGenerateBubbleCurve = register({
    name: 'generateBubbleCurveByBlock',
    label: 'timeline:contextMenu.Generate Bubble Line Chart By Block',
    visible: (session) => checkCardIsIE(session),
    perform: (session): void => {
        findLineChart(session, '2');
    },
});
export const actionGenerateCurve = register({
    name: 'generateCurveByBlock',
    label: 'timeline:contextMenu.Generate Line Chart By Block',
    visible: (session) => checkCardIsIE(session),
    perform: (session): void => {
        findLineChart(session, '1');
    },
});
