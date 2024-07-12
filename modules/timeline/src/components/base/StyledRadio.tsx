/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import * as React from 'react';
import styled from '@emotion/styled';
import { Radio } from 'antd';
import type { RadioProps } from 'antd/lib/radio';

export const StyledRadio = styled((props: RadioProps & { fontSize?: number }) => <Radio {...props}/>)`
    color: ${(props): string => props.theme.fontColor};
    font-size: ${(props): number => typeof props.fontSize === 'undefined' ? 12 : props.fontSize}px;

    .ant-radio-checked > .ant-radio-inner {
        border-color: #1890ff;
        background-color: #1890ff;
    }

    .ant-radio-input:focus + .ant-radio-inner {
        box-shadow: unset;
    }

    .ant-radio-inner {
        &::after {
            background-color: #FFF;
        }
        background-color: ${(props): string => props.theme.deviceProcessBackgroundColor};
        border: 1px solid ${(props): string => props.theme.enclosureBorder};
    }
`;
