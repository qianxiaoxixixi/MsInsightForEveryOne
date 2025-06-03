/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/

import { observer } from 'mobx-react';
import React from 'react';
import {
    queryOneKernel,
    queryAffinityOptimizer,
    affinityOptimizerColumns,
    queryAICPUOperators,
    queryACLNNOperators,
    aicpuOperatorColumns,
    aclnnOperatorColumns,
    queryAffinityAPI,
    affinityAPIColumns,
    queryOperatorFusion,
    fusionOperatorColumns,
    operatorDispatchColumns,
} from './Common';
import { BaseSummary } from './SystemView';
import { ExpertSummary } from './ExpertSummary';
import { queryExpertAnalysis, queryOperatorDispatch } from '../../api/request';
import { jumpToUnitOperator } from '../../utils';

const ExpertAnalysis = observer((props: any) => {
    return <ExpertSummary request={queryExpertAnalysis} {...props} />;
});

const AffinityAPI = observer((props: any) => {
    return <BaseSummary request={queryAffinityAPI} columns={affinityAPIColumns} {...props} />;
});

const AffinityOptimizer = observer((props: any) => {
    return <BaseSummary request={queryAffinityOptimizer} columns={affinityOptimizerColumns} {...props} />;
});

const AICPUOperator = observer((props: any) => {
    return <BaseSummary request={queryAICPUOperators} columns={aicpuOperatorColumns} {...props} />;
});

const ACLNNOperator = observer((props: any) => {
    return <BaseSummary request={queryACLNNOperators} columns={aclnnOperatorColumns} {...props} />;
});

const FusedOperator = observer((props: any) => {
    return <BaseSummary request={queryOperatorFusion} columns={fusionOperatorColumns} {...props} />;
});

const OperatorDispatch = observer((props: any) => {
    return <BaseSummary request={queryOperatorDispatch} columns={operatorDispatchColumns} {...props} />;
});

export const handleAdvisorSelected = async(rowData: any, props: any): Promise<void> => {
    const queryName = rowData.name ?? rowData.originOptimizer;
    const nsDuration = Number((rowData.duration * 1000).toFixed(0));
    const res = await queryOneKernel({
        rankId: rowData.rankId,
        name: queryName,
        timestamp: rowData.startTime,
        duration: nsDuration,
    });
    const depth = rowData.depth > res.depth ? rowData.depth : res.depth;

    jumpToUnitOperator({
        ...rowData,
        id: props.session.isFullDb as boolean ? rowData.id : res.id,
        cardId: rowData.rankId,
        timestamp: rowData.startTime,
        depth,
        duration: nsDuration,
        name: queryName,
    });
};

export const ExpertSystemView = [ExpertAnalysis, AffinityAPI, AffinityOptimizer, AICPUOperator, ACLNNOperator, FusedOperator, OperatorDispatch];
