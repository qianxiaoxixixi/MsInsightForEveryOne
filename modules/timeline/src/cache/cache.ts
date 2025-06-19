/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
*/
import { isValidSession, type ValidSession } from '../entity/session';
import { dataFunc } from './utils';

export interface Cache {
    getData: <T extends any>(session: ValidSession, params: any) => Promise<Partial<any>>;
}

export interface CacheFactory {
    create: () => Cache;
}

export type Caches = Record<any, Cache | null>;

/**
 * create a new caches object for the session wich is latest created without caches
 * @param session new session without caches
 * @returns new caches for session
 */

export const createCaches = (session: ValidSession): Caches => {
    return {};
};

const msAbsoluteTimeInterfaces: any[] = [];

export default class DicCachedEngine {
    fetchData = async <T extends any>(dataConfig: any): Promise<any> => {
        const { session, params } = dataConfig;
        if (!isValidSession(session)) {
            // wedge return emptyData as unknown as DataType<T>;
            return {};
        }
        let sessionCaches = session.caches;
        if (sessionCaches === null) {
            sessionCaches = createCaches(session);
            session.caches = sessionCaches;
        }

        let jsonData: Partial<any> = {};
        for (const key of Object.keys(params)) {
            const requestKey = key as keyof any;
            const currCache = null; // sessionCaches[requestKey];
            if (dataFunc === undefined) {
                continue;
            }
            let data;
            try {
                if (currCache === null) {
                    data = [];
                } else {
                    data = [];
                }
            } catch (e) {
                // wedge ErrorRes
                const err = e as (any | undefined);
            }
            jsonData = { ...jsonData, ...data };
        }
        return jsonData;
    };
}
