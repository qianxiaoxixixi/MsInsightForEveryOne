/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { makeStyles } from '@material-ui/core/styles';
import { Table } from 'antd';
import * as React from 'react';
import { MemoryTable, MemoryTableColumn } from '../utils/interface';

interface IProps {
    tableData: MemoryTable;
    sortColumn?: string;
    onRowSelected?: (record?: object, rowIndex?: number) => void;
    current: number;
    pageSize: number;
    onCurrentChange: (record: number) => void;
    onPageSizeChange: (record: number) => void;
    total: number;
}

const useStyles = makeStyles((theme) => ({
    row: {
        wordBreak: 'break-word',
    },
}));

const getTableColumns = function (
    columns: MemoryTableColumn[],
    sort: string | undefined,
): any {
    return columns.map((col: MemoryTableColumn) => {
        const stringCompare = (a: any, b: any): number => a[col.key].localeCompare(b[col.key]);
        const numberCompare = (a: any, b: any): number => (a[col.key] as number || 0) - (b[col.key] as number || 0);
        return {
            dataIndex: col.key,
            key: col.key,
            title: col.name,
            sorter: col.type === 'string' ? stringCompare : numberCompare,
            defaultSortOrder: sort === col.name ? ('descend' as const) : undefined,
        };
    });
};

export const AntTableChart: React.FC<IProps> = (props) => {
    const {
        tableData, sortColumn, onRowSelected, current, pageSize,
        onCurrentChange, onPageSizeChange, total,
    } = props;
    const classes = useStyles(props);

    const columns = React.useMemo(
        () => getTableColumns(tableData.columns, sortColumn),
        [ tableData.columns, sortColumn ],
    );

    // key is used to reset the Table state (page and sort) if the columns change
    const key = React.useMemo(() => `${Math.random()}`, [tableData.columns]);

    const onChange = (current: number, size: number): void => {
        onCurrentChange(current);
        onPageSizeChange(size);
    };

    const onRow = (record: object, rowIndex?: number): React.HTMLAttributes<any> => {
        return {
            onMouseEnter: (event: any) => {
                if (onRowSelected) {
                    onRowSelected(record, rowIndex);
                }
            },
            onMouseLeave: (event: any) => {
                if (onRowSelected) {
                    onRowSelected(undefined, undefined);
                }
            },
        };
    };

    return (
        <Table
            size="small"
            bordered
            columns={columns}
            dataSource={tableData.rows.map((item, index) => { return { ...item, key: `${item.name}_${index}` }; })}
            pagination={{
                current,
                pageSize,
                pageSizeOptions: [ '10', '20', '30', '50', '100' ],
                onChange,
                total,
            }}
            rowClassName={classes.row}
            key={key}
            onRow={onRow}
        />
    );
};
