/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
// Leaks Module
import { createContext, useContext } from 'react';
import { type RootStore } from '../store';
export const RootStoreContext = createContext<RootStore | undefined>(undefined);

export const useRootStore = (): RootStore => {
    const store = useContext(RootStoreContext);
    if (store === undefined) {
        throw new Error('RootStoreContext is undefined');
    }
    return store;
};
