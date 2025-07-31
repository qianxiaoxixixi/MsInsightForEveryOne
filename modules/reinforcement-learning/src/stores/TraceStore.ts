/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';
import { RootStore } from './RootStore';
import { GetTraceDataParams, GetTraceDataResults } from '@/api/types';
import { getTraceData } from '@/api';
import { message } from 'antd';

export interface SelectOption {
    label: string;
    value: string;
}

export class TraceStore {
    rootStore: RootStore;
    loading = false;
    formData: GetTraceDataParams = {};
    traceData: GetTraceDataResults | null = null;
    // 选项数据
    frameworkOptions: SelectOption[] = [
        { label: 'veRL', value: 'verl' },
    ];

    algorithmOptions: SelectOption[] = [
        { label: 'GRPO', value: 'grpo' },
    ];

    constructor(rootStore: RootStore) {
        this.rootStore = rootStore;
        makeAutoObservable(this);
    }

    setFormData(params: GetTraceDataParams): void {
        this.formData = { ...this.formData, ...params };
    }

    getTraceData = async (): Promise<void> => {
        this.loading = true;

        try {
            this.traceData = await getTraceData(this.formData);
        } catch (error) {
            message.error('请求失败，请稍后重试');
        } finally {
            this.loading = false;
        }
    };
}
