/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { useEffect } from 'react';

/**
 * make async effect cancelable
 * @param effect async effect
 * @param dependencies same as useEffect
 */
export function useAsyncEffect(effect: (isCanceled: () => boolean) => Promise<void>, dependencies?: any[]): void {
    return useEffect(() => {
        let canceled = false;
        effect(() => canceled);
        return () => { canceled = true; };
    }, dependencies);
}
