/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import React from 'react';
import styled from '@emotion/styled';

export interface LegendItem {
    label: string;
    color: string;
}

export interface LegendProps {
    items: LegendItem[];
    direction?: 'row' | 'column';
    itemSpacing?: number;
    labelColor?: string;
    fontSize?: 'small' | 'medium' | 'large';
}

const fontSizeMap: Record<NonNullable<LegendProps['fontSize']>, string> = {
    small: '12px',
    medium: '14px',
    large: '16px',
};

const LegendWrapper = styled('div')<{
    direction: 'row' | 'column';
    spacing: number;
}>`
    display: flex;
    justify-content: center;
    flex-direction: ${({ direction }) => direction};
    gap: ${({ spacing }) => spacing}px;
`;

const LegendItemWrapper = styled.div`
    display: flex;
    align-items: center;
`;

const ColorBox = styled('span')<{
    color: string;
}>`
    width: 24px;
    height: 12px;
    border-radius: 2px;
    background-color: ${({ color }) => color};
    margin-right: 6px;
    flex-shrink: 0;
`;

const Label = styled('span')<{
    color?: string;
    fontSize: 'small' | 'medium' | 'large';
}>`
    color: ${({ color, theme }) => color ?? theme.textColor};
    font-size: ${({ fontSize }) => fontSizeMap[fontSize]};
    user-select: none;
`;

export const Legend: React.FC<LegendProps> = ({
    items,
    direction = 'row',
    itemSpacing = 12,
    labelColor,
    fontSize = 'medium',
}) => {
    return (
        <LegendWrapper direction={direction} spacing={itemSpacing}>
            {items.map((item, index) => (
                <LegendItemWrapper key={index}>
                    <ColorBox color={item.color}/>
                    <Label color={labelColor} fontSize={fontSize}>
                        {item.label}
                    </Label>
                </LegendItemWrapper>
            ))}
        </LegendWrapper>
    );
};
