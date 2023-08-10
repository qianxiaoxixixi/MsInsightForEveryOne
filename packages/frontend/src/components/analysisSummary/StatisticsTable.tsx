/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { Button, Table } from 'antd';
import { DownOutlined } from '@ant-design/icons';
import React, { useEffect, useState } from 'react';
import { StringMap } from '../../utils/interface';
import { PaginationWhithPgaeData } from '../communicationAnalysis/Common';
import { computationCommunicationData } from '../../utils/mockData';

const computingStatisticsColumns = [
    {
        title: 'Accelerator Core',
        dataIndex: 'Accelerator Core',
        key: 'Accelerator Core',
    },
    {
        title: 'Accelerator Core Durations(us)',
        dataIndex: 'Accelerator Core Durations(us)',
        key: 'Accelerator Core Durations(us)',
    },

    {
        title: 'Accelerator Core Utilization',
        dataIndex: 'Accelerator Core Utilization',
        key: 'Accelerator Core Utilization',
    },
];

const computingDetailColumns = [
    {
        title: 'Name',
        dataIndex: 'Name',
    },
    {
        title: 'Type',
        dataIndex: 'Type',
    },
    {
        title: 'Start Time',
        dataIndex: 'Start Time',
    },
    {
        title: 'Duration(us)',
        dataIndex: 'Duration(us)',
    },
    {
        title: 'Wait Time(us)',
        dataIndex: 'Wait Time(us)',
    },
    {
        title: 'Block Dim',
        dataIndex: 'Block Dim',
    },
    {
        title: 'Input Shapes',
        dataIndex: 'Input Shapes',
    },
    {
        title: 'Input Data Types',
        dataIndex: 'Input Data Types',
    },
    {
        title: 'Input Formats',
        dataIndex: 'Input Formats',
    },
    {
        title: 'Output Shapes',
        dataIndex: 'Output Shapes',
    },
    {
        title: 'Output Data Types',
        dataIndex: 'Output Data Types',
    },
    {
        title: 'Output Formats',
        dataIndex: 'Output Formats',
    },
];

const communicationStatisticsColumns = [
    {
        title: 'Transport Type',
        dataIndex: 'Transport Type',
        key: 'Transport Type',
    },
    {
        title: 'Total Durations(us)',
        dataIndex: 'Total Durations(us)',
        key: 'Total Durations(us)',
    },
];

const communicationColumns = [
    {
        title: 'Communication Kernel',
        dataIndex: 'Communication Kernel',
        key: 'Communication Kernel',
    },
    {
        title: 'Start Time',
        dataIndex: 'Start Time',
        key: 'Start Time',
    },
    {
        title: 'Total Durations(us)',
        dataIndex: 'Total Durations(us)',
        key: 'Total Durations(us)',
    },
];

const communicationOverlappedColumns = [
    ...communicationColumns,
    {
        title: 'Overlapped Durations(us)',
        dataIndex: 'Overlapped Durations(us)',
        key: 'Overlapped Durations(us)',
    },
];

const communicationNotOverlappedColumns = [
    ...communicationColumns,
    {
        title: 'Not Overlapped Durations(us)',
        dataIndex: 'Not Overlapped Durations(us)',
        key: 'Not Overlapped Durations(us)',
    },
];

const getTableSet = (type: string, setExpandedKeys?: any): any => {
    const rowKeyMap: any = {
        Computing: 'Accelerator Core',
        ComputingDetail: 'Name',
        'Communication(OverLapped)': 'Transport Type',
        'Communication(Not OverLapped)': 'Transport Type',
    };
    const colMap: any = {
        Computing: computingStatisticsColumns,
        ComputingDetail: computingDetailColumns,
        'Communication(OverLapped)': communicationStatisticsColumns,
        'Communication(Not OverLapped)': communicationStatisticsColumns,
        'Communication(OverLapped)Detail': communicationOverlappedColumns,
        'Communication(Not OverLapped)Detail': communicationNotOverlappedColumns,
    };
    const rowKey = rowKeyMap[type];
    const columns = notNull(colMap[type]) ? [...colMap[type]] : [];

    if ([ 'Computing', 'Communication(OverLapped)', 'Communication(Not OverLapped)' ].includes(type)) {
        const btnCol = {
            title: 'See Details',
            key: 'action',
            render: (_: any, record: any) => (<Button type="link"
                onClick={() => {
                    setExpandedKeys((pre: string[]) => {
                        const list = [...pre];
                        const keyIndex = list.indexOf(record[rowKey]);
                        if (keyIndex === -1) {
                            list.push(record[rowKey]);
                        } else {
                            list.splice(keyIndex, 1);
                        }
                        return list;
                    });
                }}>see details<DownOutlined/></Button>),
        };
        columns.push(btnCol);
    }

    return { rowKey, columns };
};

function queryDetail(type: string, page?: any): any {
    return computationCommunicationData[type];
}

const DtetailTable = (props: any): JSX.Element => {
    const [ dataSource, setDataSource ] = useState<any[]>([]);
    const { type } = props;
    const { columns, rowKey } = getTableSet(type);
    useEffect(() => {
        updateData();
    }, []);
    const updateData = async(page?: any): Promise<void> => {
        const data = await queryDetail(type, page);
        setDataSource(data);
        setPageInfo({ total: 1000 });
    };

    const [ pageInfo, setPageInfo ] = useState({ total: 0 });
    const handlePageChange = (page: any): void => {
        updateData(page);
    };

    return <>
        <Table
            dataSource={dataSource}
            columns={columns}
            rowKey={rowKey}
            pagination={ false}
            size="small"
        />
        <PaginationWhithPgaeData handlePageChange={handlePageChange} total={pageInfo.total}/>
    </>;
};

function queryStatistics(type: string, rankId: string): any {
    return computationCommunicationData[type];
}

function notNull(val: any): boolean {
    return val !== undefined && val !== null && val !== '';
}

const typeMap: StringMap = {
    totalComputeTime: 'Computing',
    totalCommunicationNotOverLapTime: 'Communication(Not OverLapped)',
    totalCommunicationTime: 'Communication(OverLapped)',
    totalFreeTime: 'Free',
};
function getTitle(timeFlag: string): string {
    return (typeMap[timeFlag] || '') + ' Detail';
}
const StatisticsTable = (props: any): JSX.Element => {
    const { timeFlag = '', rankId = '' } = props;
    const type = typeMap[timeFlag];
    useEffect(() => {
        updateData();
    }, [ props.rankId, props.timeFlag ]);
    const updateData = async (): Promise<void> => {
        const data = await queryStatistics(type, rankId);
        setDataSource(data);
    };
    const [ dataSource, setDataSource ] = useState<any[]>([]);
    const [ expandedRowKeys, setExpandedKeys ] = useState<string[]>([]);
    const { columns, rowKey } = getTableSet(type, setExpandedKeys);

    return notNull(rankId) && notNull(type)
        ? (
            <div>
                <div className={'common-title'}>{getTitle(timeFlag)}</div>
                <Table
                    dataSource={dataSource}
                    columns={columns}
                    expandable={{
                        expandedRowRender: record => <DtetailTable type={type + 'Detail' } rankId={rankId}/>,
                        expandedRowKeys,
                        expandIcon: () => (<></>),
                    }}
                    rowKey={rowKey}
                    pagination={false}
                    size="small"
                />
            </div>)
        : <></>
    ;
};
export default StatisticsTable;
