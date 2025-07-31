/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import React, { createContext, useContext } from 'react';
import { RootStore } from './RootStore';

const rootStore = new RootStore();
const StoreContext = createContext<RootStore | null>(null);

export const StoreProvider: React.FC<{ children: React.ReactNode }> = ({ children }) => {
    return <StoreContext.Provider value={rootStore}>{children}</StoreContext.Provider>;
};

export const useStores = (): RootStore => {
    const context = useContext(StoreContext);
    if (!context) {
        throw new Error('useStores must be used within StoreProvider');
    }
    return context;
};

export * from './RootStore';
export * from './TraceStore';
export * from './SessionStore';
