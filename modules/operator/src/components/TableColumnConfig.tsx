/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import type { ColumnsType } from 'antd/es/table';
import { useTranslation } from 'react-i18next';
import { fetchColumnFilterProps } from 'ascend-resize';

export const enum OperatorGroup {
    OPERATOR = 'Operator',
    OPERATOR_TYPE = 'Operator Type',
    INPUT_SHAPE = 'Input Shape',
    HCCL_OPERATOR = 'HCCL Operator',
    HCCL_OPERATOR_TYPE = 'HCCL Operator Type',
};

const useOpl0Columns = (): ColumnsType<any> => {
    const { t } = useTranslation('operator', { keyPrefix: 'tableHead' });
    return [
        {
            title: t('Name'),
            dataIndex: 'name',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('name', 'Name'),
        },
        {
            title: t('Type'),
            dataIndex: 'type',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('type', 'Type'),
        },
        {
            title: t('AcceleratorCore'),
            dataIndex: 'accCore',
            key: 'accCore',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('accCore', 'AcceleratorCore'),
        },
        {
            title: `${t('StartTime')}(ms)`,
            dataIndex: 'startTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('Duration')}(μs)`,
            dataIndex: 'duration',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('WaitTime')}(μs)`,
            dataIndex: 'waitTime',
            sorter: true,
            ellipsis: true,
        },
    ];
};
const useOpl2Columns = (): ColumnsType<any> => {
    const { t } = useTranslation('operator', { keyPrefix: 'tableHead' });
    return [
        ...useOpl0Columns(),
        {
            title: t('BlockDim'),
            dataIndex: 'blockDim',
            sorter: true,
            ellipsis: true,
        },
        {
            title: t('InputShapes'),
            dataIndex: 'inputShape',
            sorter: true,
            ellipsis: true,
        },
        {
            title: t('InputDataTypes'),
            dataIndex: 'inputType',
            sorter: true,
            ellipsis: true,
        },
        {
            title: t('InputFormats'),
            dataIndex: 'inputFormat',
            sorter: true,
            ellipsis: true,
        },
        {
            title: t('OutputShapes'),
            dataIndex: 'outputShape',
            sorter: true,
            ellipsis: true,
        },
        {
            title: t('OutputDataTypes'),
            dataIndex: 'outputType',
            sorter: true,
            ellipsis: true,
        },
        {
            title: t('OutputFormats'),
            dataIndex: 'outputFormat',
            sorter: true,
            ellipsis: true,
        },
    ];
};
const useOpStaticColumns = (): ColumnsType<any> => {
    const { t } = useTranslation('operator', { keyPrefix: 'tableHead' });
    return [
        {
            title: t('Type'),
            dataIndex: 'opType',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('opType', 'Type'),
        },
        {
            title: t('AcceleratorCore'),
            dataIndex: 'accCore',
            key: 'accCore',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('accCore', 'AcceleratorCore'),
        },
        {
            title: t('Count'),
            dataIndex: 'count',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('TotalTime')}(μs)`,
            dataIndex: 'totalTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('AvgTime')}(μs)`,
            dataIndex: 'avgTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('MaxTime')}(μs)`,
            dataIndex: 'maxTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('MinTime')}(μs)`,
            dataIndex: 'minTime',
            sorter: true,
            ellipsis: true,
        },
    ];
};
const useOpShapeStaticColumns = (): ColumnsType<any> => {
    const { t } = useTranslation('operator', { keyPrefix: 'tableHead' });
    return [
        { title: t('Name'), dataIndex: 'opName', sorter: true, ellipsis: true, ...fetchColumnFilterProps('opName', 'Name') },
        { title: t('Shape'), dataIndex: 'inputShape', key: 'Shape', sorter: true, ellipsis: true },
        {
            title: t('AcceleratorCore'),
            dataIndex: 'accCore',
            key: 'accCore',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('accCore', 'AcceleratorCore'),
        },
        {
            title: t('Count'),
            dataIndex: 'count',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('TotalTime')}(μs)`,
            dataIndex: 'totalTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('AvgTime')}(μs)`,
            dataIndex: 'avgTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('MaxTime')}(μs)`,
            dataIndex: 'maxTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('MinTime')}(μs)`,
            dataIndex: 'minTime',
            sorter: true,
            ellipsis: true,
        },
    ];
};
const useHcclOpColumns = (): ColumnsType<any> => {
    const { t } = useTranslation('operator', { keyPrefix: 'tableHead' });
    return [
        {
            title: t('Name'),
            dataIndex: 'name',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('name', 'Name'),
        },
        {
            title: t('Type'),
            dataIndex: 'type',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('type', 'Type'),
        },
        {
            title: `${t('StartTime')}(ms)`,
            dataIndex: 'startTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('Duration')}(μs)`,
            dataIndex: 'duration',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('WaitTime')}(μs)`,
            dataIndex: 'waitTime',
            sorter: true,
            ellipsis: true,
        },
    ];
};

const useHcclOpTypeColumns = (): ColumnsType<any> => {
    const { t } = useTranslation('operator', { keyPrefix: 'tableHead' });
    return [
        {
            title: t('Type'),
            dataIndex: 'opType',
            sorter: true,
            ellipsis: true,
            ...fetchColumnFilterProps('opType', 'Type'),
        },
        {
            title: t('Count'),
            dataIndex: 'count',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('TotalTime')}(μs)`,
            dataIndex: 'totalTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('AvgTime')}(μs)`,
            dataIndex: 'avgTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('MaxTime')}(μs)`,
            dataIndex: 'maxTime',
            sorter: true,
            ellipsis: true,
        },
        {
            title: `${t('MinTime')}(μs)`,
            dataIndex: 'minTime',
            sorter: true,
            ellipsis: true,
        },
    ];
};

export const useColMap = (): any => {
    const opl0Columns = useOpl0Columns();
    const opl2Columns = useOpl2Columns();
    const opStaticColumns = useOpStaticColumns();
    const opShapeStaticColumns = useOpShapeStaticColumns();
    const hcclOpColumns = useHcclOpColumns();
    const hcclOpTypeColumns = useHcclOpTypeColumns();

    return {
        [OperatorGroup.OPERATOR]: {
            l0: opl0Columns,
            l1: opl2Columns,
            l2: opl2Columns,
        },
        [OperatorGroup.OPERATOR_TYPE]: opStaticColumns,
        [OperatorGroup.INPUT_SHAPE]: opShapeStaticColumns,
        [OperatorGroup.HCCL_OPERATOR]: hcclOpColumns,
        [OperatorGroup.HCCL_OPERATOR_TYPE]: {
            l0: hcclOpTypeColumns,
            l1: hcclOpColumns,
        },
    };
};
