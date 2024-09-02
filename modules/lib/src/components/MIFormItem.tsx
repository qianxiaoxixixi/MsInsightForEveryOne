/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import React, { type ReactNode, type CSSProperties } from 'react';
import styled from '@emotion/styled';

interface FormItemProps {
    label: string;
    colon?: boolean;
    children: ReactNode;
    style?: CSSProperties;
    labelStyle?: CSSProperties;
    contentStyle?: CSSProperties;
}

const FormItem = styled.div<Partial<FormItemProps>>`
  display: flex;
  align-items: center;
  margin-bottom: 10px;
  .label {
    margin-right: 8px;
    font-size: ${(props): string => props.theme.fontSizeBase};
  }
`;
export const MIFormItem: React.FC<FormItemProps> = (props): JSX.Element => {
    const { label, colon = true, children, style, labelStyle, contentStyle } = props;

    return (
        <FormItem style={style}>
            <div className="label" style={labelStyle}>
                {label}{colon ? ':' : ''}
            </div>
            <div className="content" style={contentStyle}>{children}</div>
        </FormItem>
    );
};

export default MIFormItem;
