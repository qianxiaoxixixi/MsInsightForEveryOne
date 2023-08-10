/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { Table } from 'antd';
import React from 'react';
import { GetPageConfigWhithAllData } from '../communicationAnalysis/Common';

const columns = [
    {
        title: 'Rank ID',
        dataIndex: 'rankId',
    },
    {
        title: 'Computing',
        dataIndex: 'totalPureCommunicationTime',
    },
    {
        title: 'Communication(Not Overlapped)',
        dataIndex: 'totalPureCommunicationTime',
    },
    {
        title: 'Communication(Overlapped)',
        dataIndex: 'totalCommunicationNotOverLapTime',
    },
    {
        title: 'Free',
        dataIndex: 'totalFreeTime',
    },
    {
        title: 'Computing Time Ratio',
        dataIndex: 'ComputingTimeRatio',
    },
    {
        title: 'Communication Time Ratio',
        dataIndex: 'CommunicationTimeRatio',
    },
];
const SummaryTable = (props: any): JSX.Element => {
    const { dataSource = [] } = props;
    return (<Table
        dataSource={dataSource}
        columns={columns}
        pagination={GetPageConfigWhithAllData(dataSource.length)}
        size="small"
    />);
};
export default SummaryTable;
