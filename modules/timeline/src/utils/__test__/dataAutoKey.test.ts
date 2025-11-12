/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { getAutoKey, AutoKey } from '../dataAutoKey';

describe('getAutoKey', () => {
    it('should return a unique key for each object', () => {
        const obj1: AutoKey<{}> = {};
        const obj2: AutoKey<{}> = {};
        const key1 = getAutoKey(obj1);
        const key2 = getAutoKey(obj2);
        expect(key1).not.toEqual(key2);
    });

    it('should return the same key for the same object', () => {
        const obj: AutoKey<{}> = {};
        const key1 = getAutoKey(obj);
        const key2 = getAutoKey(obj);
        expect(key1).toEqual(key2);
    });

    it('should return the provided key if the object has one', () => {
        const obj: AutoKey<{}> = { [Symbol('autokey')]: 'customKey' };
        const key = getAutoKey(obj);
        expect(key).toEqual('3');
    });
});
