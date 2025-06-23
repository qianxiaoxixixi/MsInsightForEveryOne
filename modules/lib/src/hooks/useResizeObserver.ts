/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { useEffect, useState } from 'react';
import ResizeObserver from 'resize-observer-polyfill';

export function useResizeObserver(el: HTMLDivElement | null): {width: number; height: number} {
    const [size, setSize] = useState({ width: 0, height: 0 });

    useEffect(() => {
        if (!el) {
            return;
        }

        const observer = new ResizeObserver(([entry]) => {
            const { inlineSize: width, blockSize: height } = entry.borderBoxSize[0];
            setSize({ width, height });
        });

        observer.observe(el);
        return () => observer.disconnect();
    }, [el]);

    return size;
}
