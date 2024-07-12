/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import React from 'react';
import type { InsightTemplate } from '../../entity/insight';
import { ReactComponent as HomePageIcon } from '../../assets/images/insights/HomePageIcon.svg';

export const entryTemplate: InsightTemplate = {
    id: 'entry',
    name: 'entry',
    source: '<internal>' as const,
    description: '',
    icon: <HomePageIcon className="homePageIcon"/>,
    units: [],
    availableUnits: [],
    isNsMode: true,
};
