/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import type { DataIndex, Key } from '../types';
import type React from 'react';

const INTERNAL_KEY_PREFIX = 'INSIGHT_TABLE_KEY';

function toArray<T>(arr: T | readonly T[]): T[] {
    if (arr === undefined || arr === null) {
        return [];
    }
    return (Array.isArray(arr) ? arr : [arr]) as T[];
}

export function getPathValue<ValueType, ObjectType extends Record<string, unknown>>(
    record?: ObjectType,
    path?: DataIndex,
): ValueType | null {
    // Skip if path is empty
    if (!path && typeof path !== 'number') {
      return record as unknown as ValueType;
    }

    const pathList = toArray(path);
    let current = record as Record<string | number, unknown>;
    for (let i = 0; i < pathList.length; i += 1) {
        if (!current) {
            return null;
        }
        const prop = pathList[i];
        current = current[prop] as Record<string | number, unknown>;
    }
    return current as ValueType;
}

interface GetColumnKeyColumn {
    key?: Key;
    dataIndex?: DataIndex;
}

export function getColumnsKey(columns: readonly GetColumnKeyColumn[]): React.Key[] {
    const columnKeys: React.Key[] = [];
    const keys: Record<React.Key, boolean> = {};

    columns.forEach(column => {
        const { key, dataIndex } = column || {};

        let mergedKey = key || toArray(dataIndex).join('-') || INTERNAL_KEY_PREFIX;
        while (keys[mergedKey]) {
            mergedKey = `${mergedKey}_next`;
        }
        keys[mergedKey] = true;

        columnKeys.push(mergedKey);
    });

    return columnKeys;
}

export function isValidValue<T>(val: T): boolean {
    return val !== null && val !== undefined;
}
