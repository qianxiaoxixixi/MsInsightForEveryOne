/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
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

import styled from '@emotion/styled';

export const ChartWrapper = styled.div`
    height: 500px;
    width: 100%;
`;

export const EmptyTipsWrapper = styled.div`
    padding: 40px;
    text-align: center;
    color: ${({ theme }) => theme.textColorSecondary};
    background-color: ${({ theme }) => theme.bgColorLight};
    border-radius: ${({ theme }) => theme.borderRadiusBase};
    margin: 20px;
`;

export const HeaderContainer = styled.div`
    display: flex;
    align-items: center;
    gap: 32px;
    padding: 8px 24px;
    margin-bottom: 8px;
`;

export const TitleContainer = styled.div`
    font-size: 14px;
    color: ${({ theme }) => theme.textColorPrimary};
    display: flex;
    align-items: center;
    gap: 8px;
`;

export const SelectContainer = styled.div`
    display: flex;
    align-items: center;
    gap: 8px;
    color: ${({ theme }) => theme.textColorPrimary};
    font-size: 14px;
`;

export const CurrentTimeContainer = styled.div`
    padding: 0 16px;
    margin-bottom: 12px;
    h3 {
        color: ${({ theme }) => theme.textColorPrimary};
    }
`;

export const ErrorBanner = styled.div`
    background-color: ${({ theme }) => theme.dangerColorLight5 || '#fef0f0'};
    border: 1px solid ${({ theme }) => theme.dangerColorLight3 || '#fcd3d3'};
    padding: 12px 16px;
    margin: 0 24px 16px 24px;
    border-radius: ${({ theme }) => theme.borderRadiusBase};
    color: ${({ theme }) => theme.dangerColor || '#c45656'};
    font-family: monospace;
    white-space: pre-wrap;
    word-break: break-all;
    max-height: 200px;
    overflow-y: auto;
`;

export const StatusBadge = styled.span<{ status: 'success' | 'error' | 'default' }>`
    display: inline-block;
    padding: 2px 8px;
    border-radius: 4px;
    font-size: 12px;
    font-weight: bold;
    color: ${({ theme, status }) => (status === 'error' ? theme.dangerColor : status === 'success' ? theme.successColor : theme.textColorSecondary)};
    background-color: ${({ theme, status }) => (status === 'error' ? theme.dangerColorLight5 : status === 'success' ? theme.successColorLight5 : theme.bgColorLight)};
    border: 1px solid ${({ theme, status }) => (status === 'error' ? theme.dangerColorLight3 : status === 'success' ? theme.successColorLight3 : theme.borderColorLight)};
`;
