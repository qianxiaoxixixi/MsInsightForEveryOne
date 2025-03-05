/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { createContext, useContext } from 'react';
import type { RootStore } from '../store';
import type { RenderEngine } from '../renderEngine';
import { ActionManager } from '../actions/manager';
export const RootStoreContext = createContext<RootStore | undefined>(undefined);

export const useRootStore = (): RootStore => {
    const store = useContext(RootStoreContext);
    if (store === undefined) {
        throw new Error('RootStoreContext is undefined');
    }
    return store;
};

export const RenderEngineContext = createContext<RenderEngine | undefined>(undefined);

export const useRenderEngine = (): RenderEngine => {
    const renderEngine = useContext(RenderEngineContext);
    if (renderEngine === undefined) {
        throw new Error('RenderEngine is undefined');
    }
    return renderEngine;
};

export const ActionManagerContext = createContext<ActionManager | undefined>(undefined);

export const useActionManager = (): ActionManager => {
    const actionManager = useContext(ActionManagerContext);
    if (actionManager === undefined) {
        throw new Error('ActionManagerContext is undefined');
    }
    return actionManager;
};
