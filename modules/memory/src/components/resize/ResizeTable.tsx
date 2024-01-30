/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import React, { cloneElement, useState, useEffect } from 'react';
import { Table } from 'antd';
import Resizor from './Resizor';

const ResizableTitle = (
    props: React.HTMLAttributes<any> & {
        index?: number;
        className: string;
        resizable: boolean;
    },
): JSX.Element => {
    const { onResize, resizable, index, ...restProps } = props;
    const th = <th {...restProps}/>;
    if (props?.className?.includes('ant-table-row-expand-icon-cell') || !resizable) {
        return th;
    }
    return cloneElement(th, {},
        [...th.props.children,
            <Resizor key={th.props.children.length} onResize={onResize}/>]);
};

const ResizeTable = (props: any): JSX.Element => {
    const { columns: propColumns, ...restProps } = props;
    const [columns, setColumns] = useState<any[]>([]);

    useEffect(() => {
        if (props.columns.length !== columns.length) {
            const newColumns = [...props.columns].map((item: any, index: number) => ({
                dataIndex: item.dataIndex,
                sorter: item.sorter,
                title: item.title,
                render: item.render,
            }));
            setColumns(newColumns);
        }
    }, [props.columns]);

    const mergeColumns = columns.map((col, index) => ({
        ...col,
        onHeaderCell: () => ({
            onResize: (deltaX: number, width: number, nextWidth?: number): void => {
                const newColumns = [...columns];
                newColumns[index] = {
                    ...newColumns[index],
                    width,
                };
                if (nextWidth !== null && nextWidth !== undefined) {
                    newColumns[index + 1] = {
                        ...newColumns[index + 1],
                        width: nextWidth,
                    };
                }
                setColumns(newColumns);
            },
            resizable: index !== props.columns.length - 1,
        }),
    }));

    return (
        <div style={{ width: '100%', overflow: 'auto' }}>
            <Table { ...restProps } columns={mergeColumns} style={{ width: 'calc(100% - 15px)' }}
                components={{
                    header: {
                        cell: ResizableTitle,
                    },
                }}
            />
        </div>
    );
};
export default ResizeTable;
