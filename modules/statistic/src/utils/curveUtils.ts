/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import { useState, useEffect, useCallback } from 'react';
import { debounce } from 'lodash';

/**
 * 界面尺寸改变
 * @returns {readonly [number]}
 */
export function useResizeEventDependency(): readonly [number] {
    const [version, setVersion] = useState(0);

    const increaseSize = useCallback(
        debounce(() => {
            setVersion((prev) => prev + 1);
        }, 100),
        [],
    );

    useEffect(() => {
        window.addEventListener('resize', increaseSize);
        return () => {
            increaseSize.cancel();
            window.removeEventListener('resize', increaseSize);
        };
    }, []);

    return [version] as const;
};
