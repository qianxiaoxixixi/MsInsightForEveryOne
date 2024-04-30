/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import React from 'react';
import BaseContainer from '../container/BaseContainer';
import BaseDescription from '../descriptions/BaseDescription';
import COLOR from './Color';
import { chartVisbilityListener, getResizeEcharts } from './EchartUtils';
export function limitInput(maxlength?: string): void {
    setTimeout(() => {
        const inputs = document.querySelectorAll('input');
        inputs.forEach(input => {
            if (input.maxLength < 0) {
                input.setAttribute('maxlength', maxlength ?? '200');
            }
        });
    });
}

let logindex: number = 0;
const logRecord: Record<number, unknown> = {};
export function log(...param: unknown[]): void {
    logRecord[logindex++ % 1000] = param;
}

export const Label = (props: {name: string;style?: object }): JSX.Element => {
    return <span style={{ margin: '0 10px', ...(props.style ?? {}) }}>{props.name ? `${props.name} :` : ''} </span>;
};

export function getSet<T extends object>(list: T[], field: keyof T): unknown[] {
    return Array.from(new Set(list.map(item => item[field])));
}

export function firstLetterUpper(word: string): string {
    return word.charAt(0).toUpperCase() + word.slice(1);
}

export { BaseContainer, BaseDescription, COLOR, chartVisbilityListener, getResizeEcharts };
