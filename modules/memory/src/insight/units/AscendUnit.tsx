import { on, unit } from '../../entity/insight';
import { Session } from '../../entity/session';
import {
    CardMetaData,
} from '../../entity/data';
import React from 'react';
const offsetConfig = (session: Session, metadata: any): JSX.Element => {
    return <></>;
};
const nsToMs = (ns: number): number => {
    return ns / 1000000;
};

export const getSliceTimeDisplay = (startTime: number | undefined): string => {
    if (startTime === undefined) {
        return '';
    }
    return `${nsToMs(startTime).toString() + ' ms'}`;
};

export const CardUnit = unit<CardMetaData>({
    name: 'Card',
    configBar: offsetConfig,
    pinType: 'move',
    renderInfo: (session: Session, metadata: { cardId: string }) => `${metadata.cardId}`,
    spreadUnits: on(
        'create',
        async (self): Promise<void> => {
        }),
});
