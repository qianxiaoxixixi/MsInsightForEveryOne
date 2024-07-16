/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { cloneDeep, throttle } from 'lodash';
import type { ElementType, KeysMatching, TimeStamp } from '../../entity/common';
import type { ValidSession } from '../../entity/session';
import { logger } from '../../utils/Logger';
import type { Cache } from '../cache';
import { dataFunc, getRange } from '../utils';
import { binarySearchFirstBig, binarySearchLastSmall } from './utils';

export interface TimeSeriesData {
    [propName: string]: unknown;
    timestamp: TimeStamp;
}

/**
 * Memory Cache for TimeSeriesData
 */
export class TimeSeriesCache<E = TimeSeriesData> implements Cache {
    maxDuration: number;
    startTime: number = 0;
    endTime: number = 0;
    // wedge key: DataKey
    key: any;
    data: E[] = [];
    getTime: (data: E) => number;

    // wedge T extends DataKey; session: ValidSession; params: DataParam<T>
    fetch = (throttle(async <T extends any>(session: ValidSession, params: any): Promise<void> => {
        const [, end] = getRange(session);
        const getActualRange = (): [ number, number ] => [this.endTime, end];
        try {
            const newData = (await dataFunc(session, getActualRange, params))[this.key] as unknown as E[];
            if (newData.length > 0) {
                const newEndTime = this.getTime(newData[newData.length - 1]);
                if (this.data.length === 0) {
                    this.data = this.data.concat(newData);
                    this.endTime = newEndTime;
                } else {
                    if (newEndTime >= this.endTime) {
                        this.data = this.data.concat(newData.filter(it => this.getTime(it) > this.endTime));
                        this.endTime = newEndTime;
                    }
                }
            }
        } catch (e) {
            // e: ErrorRes
            const err = e as (any | undefined);
            if (err !== undefined) {
                logger(`timeSeries/${this.key}`, `get error when fetching data: ${err.errorMessage}`, 'warn');
            }
        }
        if (this.startTime < this.endTime - this.maxDuration) {
            this.startTime = this.endTime - this.maxDuration;
            const startIndex = binarySearchLastSmall(this.data, this.getTime, this.startTime);
            this.data.splice(0, startIndex);
        }
    }, 500));

    // wedge key: DataKey
    constructor(maxDuration: number, key: any, getTime: (data: E) => number) {
        this.maxDuration = maxDuration;
        this.key = key;
        this.getTime = getTime;
    }

    // T extends DataKey; session: ValidSession; DataParam<T>; Promise<Partial<DataType<DataKey>>>;
    async getData<T extends any>(session: ValidSession, params: any): Promise<Partial<any>> {
        const [start, end] = getRange(session);
        if (end > this.endTime) {
            // params: DataParam<DataKey>
            await this.fetch(session, params as any);
        }
        const startIndex = binarySearchLastSmall(this.data, this.getTime, start);
        const endIndex = binarySearchFirstBig(this.data, this.getTime, end);
        return { [this.key]: cloneDeep(this.data.slice(startIndex, endIndex + 1)) };
    }
}

export const createTimeSeriesCache = <K extends KeysMatching<any, unknown[]>>(
    session: any,
    key: K,
    timeField: KeysMatching<any, number>,
): TimeSeriesCache<ElementType<any>> => {
    // TimerSeriesCache<ElementType<SampleType[K]>>
    return new TimeSeriesCache<any>(session.maxDuration, key, e => e[timeField] as unknown as number);
};
