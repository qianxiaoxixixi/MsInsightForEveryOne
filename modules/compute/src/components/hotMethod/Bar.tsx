/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2025 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 */
import React from 'react';
import styled from '@emotion/styled';
import { NOT_APPLICABLE } from './defs';

const BarContainer = styled.div`
    min-width: 100px;
    .bar {
        display: inline-block;
        height: 20px;
        float: left;
        text-align: right;
        overflow: hidden;
        color: white;
        background: ${(p): string => p.theme.primaryColor};
        border-left: 1px solid ${(p): string => p.theme.primaryColor};
        border-right: 1px solid ${(p): string => p.theme.primaryColor};
        borderRadius: 1px;
        text-overflow: ellipsis;
    }
    .width0{
        border: none;
    }
    .inside-label{
        margin-right: 2px;
    }
    .outside-label{
        margin-left: 2px;
    }
`;

export enum BarType {
    PERCENT = 0,
}
interface IProps {
    value: number;
    max?: number;
    type?: BarType;
}
// 百分比色条，2种
// 默认：使用当前值/最大值 计算百分比
// BarType。PERCENT：直接使用value作为百分比
function Bar({ value, max = 1, type }: IProps): JSX.Element {
    const valueNum = Number(value);
    const maxNum = Number(max);
    const isWrongNumber = type === BarType.PERCENT
        ? isNaN(valueNum) || valueNum < 0
        : isNaN(valueNum) || valueNum < 0 || isNaN(maxNum) || maxNum <= 0;
    let percent = 0;
    let label;
    if (!isWrongNumber) {
        percent = type === BarType.PERCENT
            ? valueNum
            : Number(((value / max) * 100).toFixed(0));
        percent = Math.min(100, percent);
        label = type === BarType.PERCENT
            ? `${valueNum}%`
            : value;
    }
    if (value === undefined || value === null) {
        return <></>;
    }
    return isWrongNumber
        ? <>{NOT_APPLICABLE}</>
        : (<BarContainer title={`${label}`}><BaseBar value={valueNum} percent={percent} label={label}/></BarContainer>);
};
export default Bar;

interface IBaseBarProps {
    value: number;
    percent: number;
    label: React.ReactNode;
}
function BaseBar({ value, percent, label }: IBaseBarProps): JSX.Element {
    // 百分比小于等于50，数字放在色条右侧，否则放在色条里面
    return percent <= 50
        ? <>
            <div className={`bar width${value}`} style={{ width: `${percent}%` }}></div>
            <span className={'outside-label'}>{label}</span>
        </>
        : <div className={'bar'} style={{ width: `${percent}%` }}>
            <span className={'inside-label'}>{label}</span>
        </div>;
}
