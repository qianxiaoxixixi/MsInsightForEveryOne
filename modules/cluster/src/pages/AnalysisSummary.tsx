/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import React from 'react';
import ComputationCommunicationOverview from '../components/summary/ComputationCommunicationOverview';
import type { Session } from '../entity/session';

const AnalysisSummary = ({ session }: { session: Session;active?: boolean }): JSX.Element => {
    return <ComputationCommunicationOverview session={session} />;
};

export default AnalysisSummary;
