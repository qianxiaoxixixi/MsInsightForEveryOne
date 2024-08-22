/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

import _ from 'lodash';

export interface communicatorContainerData {
    partitionModes: partitionMode[];
    defaultPPSize: number;
};

export interface partitionMode {
    mode: string;
    communicators: communicator[];
};

export interface communicator {
    name: string;
    ranks: number[];
    value?: string;
};

export interface tabData {
    label: string;
    key: string;
    children: JSX.Element;
};

export const titleMap = new Map([
    ['pp', 'Pipeline Parallel'],
    ['tp', 'Tensor Parallel'],
    ['dp', 'Data Parallel'],
    ['tpOrDp', 'Tensor/Data Parallel'],
]);

export const getPpContainerData = (data: communicatorContainerData, mode: string): any[] => {
    const tmp = data.partitionModes?.find(vl => vl.mode === mode);
    if (tmp === undefined) {
        return [];
    }
    const result = [];
    result.push({ label: 'All', value: 'All' });
    tmp?.communicators.forEach(item => {
        result.push({ value: item.value, label: item.value });
    });
    return result;
};

export const getAllPpStageIds = (data: communicatorContainerData): string[] => {
    const tmp = data.partitionModes?.find(vl => vl.mode === 'pp');
    if (tmp === undefined) {
        return [];
    }
    return tmp?.communicators.map(item => item.value as string);
};

const fillDpCommunicators = (values: {ppSize: number; tpSize: number; dpSize: number}, pipelineCount: number,
    pipelineSize: number, partitionModes: partitionMode[]): void => {
    if (values.dpSize <= 1) {
        return;
    }
    for (let i = 0; i < pipelineCount; i++) {
        for (let j = 0; j < values.tpSize; j++) {
            partitionModes[3].communicators.push({
                ranks: _.range((i * pipelineSize) + j, ((i + 1) * pipelineSize) + j, values.tpSize),
                name: `data${(i * values.tpSize) + j}`,
                value: `(${_.range((i * pipelineSize) + j, ((i + 1) * pipelineSize) + j, values.tpSize).join(', ')}${values.dpSize > 1 ? ')' : ',)'}`,
            });
        }
    }
};

export const generateCommunicatorData = (values: {ppSize: number; tpSize: number; dpSize: number},
    rankNum: number, defaultPPSize: number = 1): communicatorContainerData => {
    const partitionModes: partitionMode[] = [
        { mode: 'all', communicators: [] },
        { mode: 'pp', communicators: [] },
        { mode: 'tp', communicators: [] },
        { mode: 'dp', communicators: [] },
    ];
    if (rankNum === 0) {
        return { partitionModes, defaultPPSize };
    }
    if (values.ppSize !== 0 && values.tpSize !== 0) {
        const pipelineCount = values.ppSize;
        const pipelineSize = rankNum / values.ppSize;
        const modelCount = rankNum / values.tpSize;
        if (values.ppSize > 1) {
            for (let i = 0; i < pipelineSize; i++) {
                partitionModes[1].communicators.push({
                    ranks: _.range(i, rankNum, pipelineSize),
                    name: `pipeline${i}`,
                    value: `(${_.range(i, rankNum, pipelineSize).join(', ')}${pipelineSize > 1 ? ')' : ',)'}`,
                });
            }
        }
        if (values.tpSize > 1) {
            for (let i = 0; i < modelCount; i++) {
                partitionModes[2].communicators.push({
                    ranks: _.range(i * values.tpSize, (i + 1) * values.tpSize),
                    name: `model${i}`,
                    value: `(${_.range(i * values.tpSize, (i + 1) * values.tpSize).join(', ')}${values.tpSize > 1 ? ')' : ',)'}`,
                });
            }
        }
        fillDpCommunicators(values, pipelineCount, pipelineSize, partitionModes);
        partitionModes[0].communicators.push({
            ranks: _.range(0, rankNum, 1),
            name: 'All',
            value: `(${_.range(0, rankNum, 1)})`,
        });
    }
    return { partitionModes, defaultPPSize };
};

export interface rankItem {
    value: number;
    site: number[];
};
export interface tpData {
    name: string;
    key: string;
    values: rankItem[];
};

export interface dpData {
    name: string;
    key: string;
    values: tpData[];
};

export interface ppData {
    name: string;
    key: string;
    values: dpData[];
};

export const getRankData = (values: {ppSize: number; tpSize: number; dpSize: number}): ppData[] => {
    const ranksData: ppData[] = [];
    const rankNum = values.ppSize * values.tpSize * values.dpSize;
    if (values.ppSize > 0 && values.tpSize > 0 && values.dpSize > 0) {
        const pipelineSize = rankNum / values.ppSize;
        const dataSize = pipelineSize / values.dpSize;
        const tensorSize = dataSize / values.tpSize;
        for (let i = 0; i < values.ppSize; i++) {
            const ppValue: number[] = _.range(pipelineSize * i, pipelineSize * (i + 1), 1);
            const dpValue: dpData[] = [];
            for (let j = 0; j < values.dpSize; j++) {
                const dpData: number[] = ppValue.slice(dataSize * j, dataSize * (j + 1));
                const tpValue: tpData[] = [];
                for (let n = 0; n < tensorSize; n++) {
                    const rank = dpData.slice(values.tpSize * n, values.tpSize * (n + 1)).map((item, index) => ({
                        value: item,
                        site: [i, j, n, index],
                    }));
                    tpValue.push({
                        name: `tp${n}`,
                        key: `tp${n}`,
                        values: rank,
                    });
                }
                dpValue.push({
                    name: `DP ${j}`,
                    key: `dp${j}`,
                    values: tpValue,
                });
            }
            ranksData.push({
                name: `PP Stage ${i}`,
                key: `pp${i}`,
                values: dpValue,
            });
        }
    }
    return ranksData;
};
