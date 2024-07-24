/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import React, { useEffect, useState } from 'react';
import { FilterOutlined } from '@ant-design/icons';
import { Button, Input, Space } from 'antd';
import { limitInput } from '../utils/Common';
import type { ColumnType } from 'antd/es/table';
import type { FilterDropdownProps, ColumnFilterItem } from 'antd/es/table/interface';
import VirtalUl from '../utils/VirtualUl';
import type { CheckItem, ValueType } from '../utils/VirtualUl';
import i18n from '../i18n';

function FilterDropdown({ setSelectedKeys, selectedKeys, confirm, clearFilters, close, filters: originFilters }: FilterDropdownProps): JSX.Element {
    const [filters, setFilters] = useState<ColumnFilterItem[]>(originFilters ?? []);
    const [searchText, setSearchText] = useState('');
    const [selection, setSelection] = useState<React.Key[]>([]);

    const handleSelectedChange = (values: ValueType[]): void => {
        setSelection(values);
    };
    const reset = (): void => {
        const newFilters = [...filters];
        setFilters(newFilters);
        setSearchText('');
        setSelectedKeys([]);
        confirm();
    };

    useEffect(() => {
        setFilters(originFilters ?? []);
    }, [originFilters]);

    return (
        <div style={{ padding: 8, maxWidth: '400px', overflow: 'auto' }} onKeyDown={(e): void => e.stopPropagation()}>
            <div><Input allowClear maxLength={200} value={searchText} onChange={(e): void => setSearchText(e.target.value)} />
            </div>
            {filters.length > 0 && <VirtalUl items={filters as CheckItem[]} onChange={handleSelectedChange} searchText={searchText}/>}
            <Space>
                <Button type="primary" size="small" style={{ width: 90 }} onClick={(): void => {
                    setSelectedKeys(selection);
                    confirm();
                }}>{i18n.t('buttonText:Search')}</Button>
                <Button size="small" style={{ width: 90 }} onClick={(): void => { reset(); }} >{i18n.t('buttonText:Reset')}</Button>
            </Space>
        </div>
    );
}
export const getColumnSearchProps = <T extends Record<string, unknown>>(clearFilters?: () => void): ColumnType<T> => {
    return ({
        filterDropdown: (props: FilterDropdownProps): React.ReactNode => (
            <FilterDropdown {...props} clearFilters={clearFilters}/>),
        filterIcon: (filtered: boolean) => (
            <FilterOutlined />
        ),
        onFilterDropdownOpenChange: (visible: boolean): void => {
            if (visible) {
                limitInput();
            }
        },
    });
};
