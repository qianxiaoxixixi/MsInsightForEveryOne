/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */

import React from 'react';
import type { ErrorInfo, ReactNode } from 'react';
import type { Session } from '../../entity/session';
import { logger } from '../../utils/Logger';

interface ErrorProps {
    children: JSX.Element;
}

interface ErrorState {
    hasError: boolean;
}

export type UncaughtError = Error;

export abstract class ErrorBoundaryBase<P = Record<string, unknown>> extends React.Component<P & ErrorProps, ErrorState> {
    state = { hasError: false };
    static getDerivedStateFromError(): Record<string, unknown> {
        // 更新状态，使下一次渲染显示回退 UI
        return { hasError: true };
    }

    componentDidCatch(error: Error, info: ErrorInfo): void {
        // 1、打印日志到底座log
        // 2、看DevEco有没有error report通道可以接入
        logger('insightError', `componentStack:
        ${info.componentStack}`);
        this.handleError(error);
    }

    reset(): void {
        this.setState({ hasError: false });
    }

    render(): ReactNode {
        if (this.state.hasError) {
            return this.fallBackUI(this.props.children.props.session);
        }

        return this.props.children;
    }

    abstract handleError(error: UncaughtError): void;

    abstract fallBackUI(session: Session): JSX.Element;
}
