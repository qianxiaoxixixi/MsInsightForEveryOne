/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import React, { useState, useEffect, useCallback } from 'react';
import { debounce } from 'lodash';

/**
 * 界面尺寸改变
 * @returns {readonly [number]}
 */
export function useResizeEventDependency(): readonly [number] {
    const [ version, setVersion ] = useState(0);

    const increaseSize = useCallback(
        debounce(() => {
            setVersion((prev) => prev + 1);
        }, 100),
        [],
    );

    useEffect(() => {
        window.addEventListener('resize', increaseSize);

        return () => {
            window.removeEventListener('resize', increaseSize);
        };
    }, []);

    return [version] as const;
};

/**
 * 二分搜索
 * @param arr 搜索的数组
 * @param key 搜索的key值
 * @param compareFun 比较函数
 * @returns {number} 要搜索值的索引
 */
export function binarySearch(arr: any[], key: any, compareFun: Function): number {
    let low = 0;
    let high = arr.length - 1;
    while (low <= high) {
        const mid = Math.round((high + low) / 2);
        const cmp = compareFun(key, arr[mid]);
        if (cmp > 0) {
            low = mid + 1;
        } else if (cmp < 0) {
            high = mid - 1;
        } else {
            return mid;
        }
    }
    return -1;
}
