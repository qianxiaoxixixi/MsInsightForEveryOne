/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import React, { useEffect, useMemo, useState } from 'react';
import i18n from 'ascend-i18n';
import { useTranslation } from 'react-i18next';
import type { TFunction } from 'i18next';
import { runInAction } from 'mobx';
import { observer } from 'mobx-react';
import { ResizeTable, fetchColumnFilterProps } from 'ascend-resize';
import { Tooltip } from 'ascend-components';
import { Session } from '../entity/session';
import { getEventTableData } from './dataHandler';
import { generateJsonShow } from '../utils/utils';

const getTableColumns = (t: TFunction, session: Session): any => {
    return session.eventsTableHeader.map((col: any) => {
        const item = {
            dataIndex: col.key,
            key: col.key,
            title: t(col.name, { defaultValue: col.name, keyPrefix: 'tableHead' }),
            sorter: col.sortable,
            ellipsis: {
                showTitle: false,
            },
            showSorterTooltip: t(col.name, { keyPrefix: 'tableHeadTooltip', defaultValue: '' }) === ''
                ? true
                : { title: t(col.name, { keyPrefix: 'tableHeadTooltip' }) },
            render: (text: string): React.ReactNode => (<Tooltip
                title={col.key === 'attr' && text ? generateJsonShow(text) : text || ''}
                placement="top"
            >
                {text || ''}
            </Tooltip>),
        };
        if (col.searchable) {
            return { ...item, ...fetchColumnFilterProps(col.key, col.name.replace(' ', '')) };
        } else {
            return item;
        }
    });
};

const EventsTable = observer(({ session }: { session: Session }): React.ReactElement => {
    const { t } = useTranslation('leaks');
    const {
        deviceId, eventsTableData, eventsTableHeader, eventsCurrentPage, tableKey,
        eventsPageSize, eventsTotal, eventsOrder, eventsOrderBy, eventsFilters, maxTime, minTime,
    } = session;
    const [loading, setLoading] = useState(false);
    const defaultDataSource = (process.env.NODE_ENV === 'development' ? [{}] : []);
    const columns = useMemo(() => getTableColumns(t, session), [JSON.stringify(eventsTableHeader), t]);
    const onTableChange = (pagination: any, filters: any, sorter: any, extra: any): void => {
        if (extra.action === 'sort') {
            runInAction(() => {
                if (sorter.order === undefined) {
                    session.eventsOrder = '';
                } else {
                    session.eventsOrder = sorter.order !== 'ascend';
                }
                session.eventsOrderBy = sorter.field;
            });
        }
        if (extra.action === 'filter') {
            const newFilters: { [key: string]: string } = {};
            const oldFilters = Object.keys(filters);
            oldFilters.forEach((key: string) => {
                if (filters[key]?.[0] !== undefined) {
                    newFilters[key] = filters[key]?.[0];
                }
            });
            runInAction(() => {
                session.eventsFilters = newFilters;
            });
        }
    };
    const onChange = (newCurrent: number, newPageSize: number): void => {
        runInAction(() => {
            session.eventsCurrentPage = newCurrent;
            session.eventsPageSize = newPageSize;
        });
    };
    useEffect(() => {
        if (deviceId === '' || maxTime === 0 || maxTime === undefined) return;
        setLoading(true);
        getEventTableData(session);
        setLoading(false);
    }, [deviceId, maxTime, minTime, eventsCurrentPage, eventsPageSize, eventsOrder, eventsOrderBy, JSON.stringify(eventsFilters)]);
    return (
        <>
            <ResizeTable
                columns={columns}
                dataSource={eventsTableData.length === 0 ? defaultDataSource : eventsTableData.map((item: any, index: number) => ({ ...item, key: `${item.name}_${index}` }))}
                onChange={onTableChange}
                pagination={{
                    current: eventsCurrentPage,
                    pageSize: eventsPageSize,
                    pageSizeOptions: [10, 20, 30, 50, 100],
                    onChange,
                    total: eventsTotal,
                    showTotal: (totalNum: number): string => i18n.t('PaginationTotal', { total: totalNum }),
                    showQuickJumper: true,
                }}
                scroll={{ x: 150 * columns.length }}
                style={{ height: 400 }}
                loading={loading}
                key={`${tableKey}_Events`}
            />
        </>
    );
});

export default EventsTable;
