/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import styled from '@emotion/styled';

export const Line = styled.div<{ lineShow: string; offset: number }>`
    position: absolute;
    top: 20px;
    width: 1.2px;
    height:260px;
    background-color: #999;
    pointer-events: none;
    display: ${(props): string => props.lineShow};
    transform: translateX(${(props): number => props.offset}px);
    z-index: 10;
    border-left: 1px dashed #999;
`;

export const initLine = (mouseEnter: any, mouseMove: any, mouseLeave: any): void => {
    const funcContent = document.getElementById('funcContent');
    const barContent = document.getElementById('barContent');
    [funcContent, barContent].forEach(content => {
        content?.addEventListener('mouseenter', mouseEnter);
        content?.addEventListener('mousemove', mouseMove);
        content?.addEventListener('mouseleave', mouseLeave);
    });
};

export const cancelLine = (mouseEnter: any, mouseMove: any, mouseLeave: any): void => {
    const funcContent = document.getElementById('funcContent');
    const barContent = document.getElementById('barContent');
    [funcContent, barContent].forEach(content => {
        content?.removeEventListener('mouseenter', mouseEnter);
        content?.removeEventListener('mousemove', mouseMove);
        content?.removeEventListener('mouseleave', mouseLeave);
    });
};
