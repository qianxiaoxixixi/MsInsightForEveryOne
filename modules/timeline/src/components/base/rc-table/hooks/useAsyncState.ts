/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import React from 'react';

export type Updater<State> = (prev: State) => State;

export const useAsyncState = <State>(defaultState: State): [State, (updater: Updater<State>) => void] => {
    const [, forceRender] = React.useState({});
    const immutableStateRef = React.useRef(defaultState);
    const lastExecuteRef = React.useRef<Promise<void>>();
    const updaterBatchRef: React.MutableRefObject<Array<Updater<State>>> = React.useRef([]);

    React.useEffect(() => {
        return () => { lastExecuteRef.current = undefined; };
    }, []);

    const setAsyncState = (updater: Updater<State>): void => {
        const asyncExecutor = Promise.resolve();
        lastExecuteRef.current = asyncExecutor;
        updaterBatchRef.current.push(updater);

        (async (): Promise<void> => {
            await asyncExecutor;
            if (lastExecuteRef.current !== asyncExecutor) {
                return;
            }
            const prevBatch = updaterBatchRef.current;
            const prevState = immutableStateRef.current;
            updaterBatchRef.current = [];

            prevBatch.forEach(update => {
                immutableStateRef.current = update(immutableStateRef.current);
            });

            lastExecuteRef.current = undefined;
            if (prevState !== immutableStateRef.current) {
                forceRender({});
            };
        })();
    };

    return [immutableStateRef.current, setAsyncState];
};
