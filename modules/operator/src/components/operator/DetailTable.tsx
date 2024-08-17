/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { ResizeTable } from 'ascend-resize';
import React, { useState, useEffect } from 'react';
import { useTranslation } from 'react-i18next';
import { Button } from 'ascend-components';
import { DownOutlined } from '@ant-design/icons';
import { getPageConfigWithPageData } from '../Common';
import { type ConditionType, type FilterType } from './Filter';
import { queryOperators, queryOperatorsInStatic, queryOperatorStatic } from '../RequestUtils';
import { runInAction } from 'mobx';
import type { Session } from '../../entity/session';
import CollapsiblePanel from 'ascend-collapsible-panel';
import { OperatorGroup, useColMap } from '../TableColumnConfig';

interface FullConditionType {
    rankId: string ;
    group: string;
    topK: number;
    current: number;
    pageSize: number;
    field: string;
    order: string;
    type: string[];
    opType: string[];
    name: string[];
    opName: string[];
    accCore: string[];
};

const OperatorTable = ({ condition, filterType, opType, accCore, opName, inputShape, session }:
{condition: ConditionType;filterType: FilterType;opType?: string;accCore?: string;opName?: string;inputShape?: string;session: Session}): JSX.Element => {
    return <BaseTable
        condition={condition}
        filterType={filterType}
        opType={opType}
        accCore={accCore}
        opName={opName}
        inputShape={inputShape}
        session={session}
    />;
};

const OperatorTypeTable = ({ condition, filterType, session }: {condition: ConditionType;filterType: FilterType;session: Session}): JSX.Element => {
    return <BaseTable condition={condition} filterType={filterType} session={session} />;
};

const defaultPage = { current: 1, pageSize: 10, total: 0 };
const defaultSorter = { field: '', order: '' };
const defaultFilters = { type: [], opType: [], name: [], opName: [], accCore: [] };

const getCols = ({ group, columnLevel, btnCol, colMap, condition, isExpend }:
{group: string;columnLevel: string;btnCol: any;colMap: any;condition: ConditionType;isExpend: boolean}): any[] => {
    switch (group) {
        case OperatorGroup.OPERATOR:
            return colMap[group][columnLevel] ?? colMap[group].l2;
        case OperatorGroup.HCCL_OPERATOR:
            return colMap[group];
        case OperatorGroup.HCCL_OPERATOR_TYPE:
            if (columnLevel === undefined) {
                return [...colMap[group].l0 ?? [], btnCol];
            }
            return colMap[group][columnLevel];
        default:
            return [...colMap[group] ?? [], btnCol];
    }
};

const setFilterTypes = (fullCondition: FullConditionType): string[] => {
    const filterColumn = ['type', 'opType', 'name', 'opName', 'accCore'];
    const filterTypes: string[] = [];
    Object.keys(fullCondition).forEach(key => {
        const filterValue = fullCondition[key as keyof FullConditionType];
        if (filterColumn.includes(key) && filterValue != null) {
            if (Array.isArray((filterValue)) && filterValue.length > 0) {
                filterTypes.push(JSON.stringify({ columnName: key, value: filterValue[0] }));
            }
        }
    });
    return filterTypes;
};

const queryOperatorData = async ({ condition, fullCondition, filterTypes }:
{condition: ConditionType;fullCondition: FullConditionType;filterTypes: string[]}): Promise<any> => {
    let res;
    if (condition.group === OperatorGroup.OPERATOR || condition.group === OperatorGroup.HCCL_OPERATOR) {
        const param = { ...fullCondition, orderBy: fullCondition.field, filters: filterTypes };
        res = await queryOperators(param);
    } else {
        const param = { ...fullCondition, orderBy: fullCondition.field, filters: filterTypes };
        res = await queryOperatorStatic(param);
    }
    return res;
};

const queryOperatorDetailData = async ({ fullCondition, filterTypes, opType, opName, accCore, inputShape }:
{fullCondition: FullConditionType;filterTypes: string[];opType?: string;opName?: string;accCore?: string;inputShape?: string}): Promise<any> => {
    const param = { ...fullCondition, orderBy: fullCondition.field, filters: filterTypes, opType: opType ?? '', opName, shape: inputShape ?? '', accCore: accCore ?? '' };
    return await queryOperatorsInStatic(param);
};

// eslint-disable-next-line max-lines-per-function
const BaseTable = ({ condition, filterType, opType, accCore, opName, inputShape, session }:
{condition: ConditionType;filterType: FilterType;opType?: string;accCore?: string;opName?: string;inputShape?: string;session: Session}): JSX.Element => {
    const { t } = useTranslation();
    const [cols, setCols] = useState<any[]>(useColMap()[OperatorGroup.OPERATOR].l0);
    const [page, setPage] = useState(defaultPage);
    const [sorter, setSorter] = useState(defaultSorter);
    const [filters, setFilters] = useState(defaultFilters);
    const [tableData, setTableData] = useState<any[]>([]);
    const rowKey = 'rowKey';
    const [expandedRowKeys, setExpandedKeys] = useState<string[]>([]);
    const [loading, setLoading] = useState(false);
    const [fullCondition, setFullCondition] = useState<FullConditionType>({
        current: 1, pageSize: 10, field: '', order: '', group: '', rankId: '', topK: 0, type: [], opType: [], name: [], opName: [], accCore: [],
    });
    const btnCol = {
        title: t('Details'),
        width: 115,
        key: 'action',
        render: (_: any, record: any) => (<Button type="link"
            onClick={(): void => {
                setExpandedKeys((pre: any) => {
                    const list = [...pre];
                    const keyIndex = list.indexOf(record[rowKey]);
                    if (keyIndex === -1) {
                        list.push(record[rowKey]);
                    } else {
                        list.splice(keyIndex, 1);
                    }
                    return list;
                });
            }}>{t('SeeMore', { ns: 'buttonText' })}<DownOutlined/></Button>),
    };
    const colMap = useColMap();
    const updateData = async(): Promise<void> => {
        let isExpend = false;
        const filterTypes = setFilterTypes(fullCondition);
        let res;
        if (opType !== undefined || opName !== undefined || accCore !== undefined) {
            // 展开算子
            isExpend = true;
            res = await queryOperatorDetailData({ fullCondition, filterTypes, opType, opName, accCore, inputShape });
        } else {
            res = await queryOperatorData({ condition, fullCondition, filterTypes });
        };
        if (res === null || res === undefined) {
            return;
        }
        const { data, total, level } = res;
        data.forEach((item: any, index: number) => {
            item.rowKey = condition.group + String((page.pageSize * page.current) + index);
        });
        setTableData(data);
        setPage({ ...page, total });
        let group = opType !== undefined ? OperatorGroup.OPERATOR : condition.group;
        let columnLevel = level;
        if (condition.group === OperatorGroup.HCCL_OPERATOR_TYPE && isExpend) {
            group = OperatorGroup.HCCL_OPERATOR_TYPE;
            columnLevel = 'l1';
        }
        const columns = getCols({ group, columnLevel, btnCol, colMap, condition, isExpend });
        setCols(columns);
        runInAction(() => {
            session.total = total;
        });
    };

    const updateTable = async (): Promise<void> => {
        setLoading(true);
        try {
            await updateData();
        } finally {
            setExpandedKeys([]);
            setLoading(false);
        }
    };

    const updateFullCondition = (obj: FullConditionType): void => {
        setTimeout(() => {
            const newFullCondition = { ...fullCondition };
            const keys = ['group', 'rankId', 'topK', 'current', 'pageSize',
                'field', 'order', 'type', 'opType', 'name', 'opName', 'accCore'];
            Object.keys(obj).forEach(key => {
                if (keys.includes(key)) {
                    Object.assign(newFullCondition, { [key]: obj[key as keyof FullConditionType] });
                }
            });
            setFullCondition(newFullCondition);
        });
    };

    useEffect(() => {
        if (condition.rankId === '') {
            setTableData([]);
            runInAction(() => {
                session.total = 0;
            });
            return;
        }
        updateTable();
    }, [JSON.stringify(fullCondition), t]);

    useEffect(() => {
        setSorter(defaultSorter);
        setPage(defaultPage);
        setFilters(defaultFilters);
        updateFullCondition({ ...defaultSorter, ...defaultPage, ...defaultFilters, ...condition });
    }, [condition.group]);

    useEffect(() => {
        updateFullCondition({ ...sorter, ...page, ...filters, ...condition });
    }, [page.current, page.pageSize, sorter.field, sorter.order,
        filters.type, filters.opType, filters.name, filters.opName, filters.accCore,
        condition.rankId, condition.topK]);
    return <ResizeTable
        size="small"
        minThWidth={50}
        loading={loading}
        columns={cols}
        dataSource={tableData}
        pagination={getPageConfigWithPageData(page, setPage)}
        onChange={(pagination: any, newFilters: any, newSorter: any, extra: any): void => {
            if (extra.action === 'sort') {
                setSorter(newSorter.order === undefined ? { field: '', order: '' } : newSorter);
            }
            if (extra.action === 'filter') {
                setFilters(newFilters === undefined ? {} : newFilters);
            }
        }
        }
        rowKey={rowKey}
        expandable={condition.group !== OperatorGroup.OPERATOR && condition.group !== OperatorGroup.HCCL_OPERATOR
            ? {
                expandedRowRender: (record: any) => <OperatorTable
                    condition={condition}
                    filterType={filterType}
                    opName={record.opName}
                    opType={record.opType}
                    inputShape={record.inputShape}
                    accCore={record.accCore}
                    session={session}
                />,
                expandedRowKeys,
                expandIcon: () => (<></>),
            }
            : false}
    />;
};

const DetailTable = ({ condition, filterType, session }: {condition: ConditionType;filterType: FilterType;session: Session}): JSX.Element => {
    const { t } = useTranslation('operator');
    let table;
    switch (condition.group) {
        case OperatorGroup.OPERATOR:
        case OperatorGroup.HCCL_OPERATOR:
            table = <OperatorTable condition={condition} filterType={filterType} session={session}/>;
            break;
        case OperatorGroup.OPERATOR_TYPE:
        case OperatorGroup.HCCL_OPERATOR_TYPE:
            table = <OperatorTypeTable condition={condition} filterType={filterType} session={session}/>;
            break;
        default:
            table = <BaseTable condition={condition} filterType={filterType} session={session}/>;
            break;
    }
    return <CollapsiblePanel title={t('sessionTitle.OperatorDetails')} secondary>
        {table}
    </CollapsiblePanel>;
};

export default DetailTable;
