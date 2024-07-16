/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */

import { ErrorBoundaryBase } from './ErrorBoundaryBase';
import type { UncaughtError } from './ErrorBoundaryBase';
import { logger } from '../../utils/Logger';
import React from 'react';
import type { ReactNode } from 'react';
import styled from '@emotion/styled';
import type { Session } from '../../entity/session';

interface FallBackProps {
    height?: number;
    width?: number;
    className?: string;
    phase?: string;
};
export class ChartErrorBoundary extends ErrorBoundaryBase<FallBackProps> {
    handleError = (error: UncaughtError): void => {
        // 根据不同子组件，按需设置session状态为error、右下角弹出提示、自定义状态/重置数据尝试恢复现场等等
        logger('insightError', `error in chart：
        ${error.stack}`, 'warn');
    };

    fallBackUI = (session: Session): JSX.Element => <ChartErrorContainer className={this.props.className}
        height={this.props.height}
        width={this.props.width}>No
        Data</ChartErrorContainer>;

    render = (): ReactNode => {
        if (this.state.hasError || this.props.phase === 'error') {
            return this.fallBackUI(this.props.children.props.session);
        }
        return this.props.children;
    };
}

const ChartErrorContainer = styled.div<{height?: number; width?: number}>`
    display: flex;
    height: ${(p): string => {
        if (p.height === undefined) {
            return '100%';
        }
        return `${p.height}px`;
    }};
    width: 100%;
    justify-content: center;
    align-items: center;
    background-color: ${(props): string => props.theme.chartWrongBGColor};
`;
