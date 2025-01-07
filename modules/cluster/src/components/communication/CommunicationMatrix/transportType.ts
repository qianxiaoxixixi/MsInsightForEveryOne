/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import type { PiecewiseVisualMapOption } from 'echarts/types/dist/shared';
import { COLOR } from '../../Common';

export const allTransporType = ['HCCS', 'PCIE', 'RDMA', 'LOCAL', 'SIO'];
export const getTransporTypeNumber = (type: string): number => allTransporType.indexOf(type);
export const getTransportTypeName = (num: number): string => allTransporType[num] ?? '';

export const transportTypeVisualMap: PiecewiseVisualMapOption = {
    type: 'piecewise',
    orient: 'horizontal',
    left: 'center',
    bottom: '0',
    splitNumber: 1,
    pieces: [
        { value: 0, label: allTransporType[0], color: COLOR.BAND_0 },
        { value: 1, label: allTransporType[1], color: COLOR.BAND_1 },
        { value: 2, label: allTransporType[2], color: COLOR.BAND_2 },
        { value: 3, label: allTransporType[3], color: COLOR.BAND_3 },
        { value: 4, label: allTransporType[4], color: COLOR.BAND_4 },
    ],
    textStyle: { color: COLOR.GREY_40 },
    dimension: 2,
};
