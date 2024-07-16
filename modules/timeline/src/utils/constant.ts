/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
export type Phase = 'configuring' | 'waiting' | 'recording' | 'analyzing' | 'download' | 'error';

export const stateTexts: Record<Phase, string> = {
    waiting: 'Initializing...',
    recording: 'Recording...',
    analyzing: 'Analyzing...',
    configuring: '',
    download: '',
    error: '',
};
