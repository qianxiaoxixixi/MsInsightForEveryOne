/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/

import { observer } from 'mobx-react';
import { runInAction } from 'mobx';
import React, { useEffect, useState } from 'react';
import { useTranslation } from 'react-i18next';
import {
    useKernelDetails,
    queryKernelDetails,
    queryOneKernel,
    getPageData,
    querySystemViewDetails,
    pythonApiSummaryColumns,
    layerTypes,
} from './Common';
import type { CardMetaData, ThreadMetaData } from '../../entity/data';
import type { InsightUnit } from '../../entity/insight';
import { getDetailTimeDisplay, ThreadUnit } from '../../insight/units/AscendUnit';
import { colorPalette, getTimeOffset } from '../../insight/units/utils';
import { calculateDomainRange } from '../CategorySearch';
import { hashToNumber } from '../../utils/colorUtils';
import { Button } from 'ascend-components';
import { ResizeTable } from 'ascend-resize';
import { StyledEmpty } from 'ascend-utils';
import { DETAIL_HEADER_HEIGHT_ETC_PX, BaseSummary } from './SystemView';
import { OverallMetrics } from './OverallMetrics';

const filterColumn = [
    'name', 'type', 'acceleratorCore', 'taskId', 'inputShapes', 'inputDataTypes',
    'inputFormats', 'outputShapes', 'outputDataTypes', 'outputFormats',
];

const handleSelected = async(rowData: any, props: any): Promise<void> => {
    const res = await queryOneKernel({
        rankId: props.rankId,
        name: rowData.name,
        timestamp: rowData.startTime,
        duration: Number((rowData.duration * 1000).toFixed(0)),
    });
    runInAction(() => {
        props.session.locateUnit = {
            target: (unit: any): boolean => {
                return unit instanceof ThreadUnit && unit.metadata.cardId === props.rankId &&
                    unit.metadata.threadId === res.threadId && unit.metadata.processId === res.pid;
            },
            onSuccess: (unit: InsightUnit): void => {
                const startTime = rowData.startTime - getTimeOffset(props.session, unit.metadata as ThreadMetaData);
                // 此处duration单位为us,计算和跳转时需要转换为ns
                const [rangeStart, rangeEnd] = calculateDomainRange(props.session, startTime, Number((rowData.duration * 1000).toFixed(0)));
                props.session.domainRange = { domainStart: rangeStart, domainEnd: rangeEnd };
                props.session.selectedData = {
                    id: props.session.isFullDb as boolean ? rowData.id : res.id,
                    startTime,
                    name: rowData.name,
                    color: colorPalette[hashToNumber(rowData.name, colorPalette.length)],
                    duration: Number((rowData.duration * 1000).toFixed(0)),
                    depth: res.depth,
                    threadId: res.threadId,
                    processId: res.pid,
                    cardId: props.rankId,
                    startRecordTime: props.session.startRecordTime,
                    metaType: res.pid,
                    showDetail: false,
                };
            },
        };
    });
};

const defaultFilters = {
    name: [],
    type: [],
    accCore: [],
    taskId: [],
    inputShapes: [],
    inputDataTypes: [],
    inputFormats: [],
    outputShapes: [],
    outputDataTypes: [],
    outputFormats: [],
};

const KernelDetails = observer((props: any) => {
    const defaultPage = { current: 1, pageSize: 10, total: 0 };
    const defaultSorter = { field: 'duration', order: 'descend' };
    const [dataSource, setDataSource] = useState<any[]>([]);
    const [page, setPage] = useState(defaultPage);
    const [sorter, setSorter] = useState(defaultSorter);
    const [filters, setFilters] = useState(defaultFilters);
    const [rowData, setRowData] = useState<any>({});
    const kernelDetails = useKernelDetails();
    const { t } = useTranslation('timeline', { keyPrefix: 'tableHead' });

    const status = props.session.units.find((unit: any) => (unit.metadata as CardMetaData).cardId === props.rankId)?.phase;
    useEffect(() => {
        updateData(page, sorter, filters);
    }, [sorter, filters, props.rankId]);
    useEffect(() => {
        if (status === 'download') {
            updateData(page, sorter, filters);
        }
    }, [status]);
    const updateData = async(pages: any, sorters: {field: string;order: string}, filtersConditions: any): Promise<void> => {
        if (props.rankId === undefined) {
            setDataSource([]);
            setPage(defaultPage);
            return;
        }
        const filterTypes: string[] = [];
        Object.keys(filtersConditions).forEach(key => {
            const filterValue = filtersConditions[key];
            if (filterColumn.includes(key) && filterValue != null) {
                if (Array.isArray((filterValue)) && filterValue.length > 0) {
                    filterTypes.push(JSON.stringify({ columnName: key, value: filterValue[0] }));
                }
            }
        });
        const res = await queryKernelDetails({
            rankId: props.rankId,
            pageSize: pages.pageSize,
            current: pages.current,
            orderBy: sorters.field === 'startTimeLabel' ? 'startTime' : sorters.field ?? defaultSorter.field,
            order: sorters.order ?? defaultSorter.order,
            coreType: '',
            filterCondition: filterTypes,
        });
        const timestampoffset = getTimeOffset(props.session, props.rankId);
        const data = res.kernelDetails.map((item: {
            startTimeLabel: string;
            startTime: number;}) => {
            item.startTimeLabel = getDetailTimeDisplay(item.startTime - timestampoffset);
            return item;
        });
        setDataSource(data);
        setPage({ ...page, total: res.count });
    };

    const colums = [
        ...kernelDetails,
        {
            title: t('Click To Timeline'),
            dataIndex: 'click',
            key: 'click',
            ellipsis: true,
            render: (_: any, record: any): JSX.Element => (<Button type="link"
                onClick={(): void => {
                    setRowData(record as any);
                }}>{t('Click')}</Button>),
        },
    ];

    useEffect(() => {
        if (rowData.name === null || rowData.name === undefined) {
            return;
        }
        handleSelected(rowData, props);
    }, [rowData]);
    return (
        (status === 'download' || props.rankId === undefined)
            ? <ResizeTable
                onChange={(pagination: any, newFilters: any, newSorter: any): void => {
                    setSorter(newSorter);
                    setFilters(newFilters);
                }}
                pagination={getPageData(page, setPage)}
                dataSource={dataSource}
                columns={colums}
                scroll={{ y: props.bottomHeight - DETAIL_HEADER_HEIGHT_ETC_PX }}
                rowClassName={(record: any): string => {
                    return record.id === rowData.id ? 'selected-row' : 'click-able';
                }}
                size="small"/>
            : <div style={{ display: 'flex', height: '100%' }}>
                <StyledEmpty style={{ margin: 'auto' }}/>
            </div>
    );
});

export const StatsSystemView = [OverallMetrics, ...layerTypes.map((type) => {
    return observer((props: any) => {
        return (
            <BaseSummary
                layerType={type}
                request={querySystemViewDetails}
                isStats={true}
                columns={pythonApiSummaryColumns}
                {...props}
            />
        );
    });
}), KernelDetails];
