/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import React, { cloneElement, useState, useEffect, useRef, useMemo } from 'react';
import { Table } from 'antd';
import type { ColumnsType } from 'antd/es/table';
import { isArray } from 'lodash';
import Resizor from './Resizor';
import { getColumnSearchProps } from './ColumnFilterWithSelection';
import { limitInput } from '../utils/Common';
import { useWatchVirtualRender } from '../utils/VirtualRenderUtils';
interface ExtendsColumnType {minWidth?: number};

interface IResizableTitleProps extends React.ReactElement {
    index?: number;
    className: string;
    resizable: boolean;
    onResize: (deltaX: number, width: number, nextWidth?: number) => void;
}
const resizableTitle: React.FC<IResizableTitleProps> = (props) => {
    const { onResize, resizable, index, ...restProps } = props;
    const th: React.ReactElement = <th {...restProps}/> as React.ReactElement;
    if (props?.className?.includes('ant-table-row-expand-icon-cell') || !resizable) {
        return th;
    }
    return cloneElement(th, {},
        [...th.props.children,
            <Resizor key={th.props.children.length} onResize={onResize}/>]);
};

interface Iprop<T> {
    [prop: string]: object | number | string | boolean | undefined | object[];
    columns: ColumnsType<T> ;
    dataSource: T[];
    size?: 'small' | 'middle' | 'large' ;
    id?: string;
    variableTotalWidth?: boolean;
    minThWidth?: number;
    style?: object;
    virtual?: boolean;
    scroll?: {x?: number;y?: number;rowHeight?: number};
    pagination?: {showSizeChanger: boolean};
}

// ============================ Resize ============================
const getResizeColumns = ({ columns, index, width, nextWidth, minThWidth, variableTotalWidth }:
{columns: any[];index: number; width: number; nextWidth?: number;minThWidth: number;variableTotalWidth: boolean}): any[] => {
    const newColumns = [...columns];
    newColumns[index] = {
        ...newColumns[index],
        width: Math.max(width, minThWidth, (columns[index] as ExtendsColumnType).minWidth ?? 0),
    };
    if (nextWidth !== null && nextWidth !== undefined && !variableTotalWidth) {
        newColumns[index + 1] = {
            ...newColumns[index + 1],
            width: Math.max(nextWidth, minThWidth, (columns[index + 1] as ExtendsColumnType).minWidth ?? 0),
        };
    }
    return newColumns;
};

// ============================ virtual ============================
const getVirtualElement = (dom: Element): [Element | null, Element | null] => {
    const box = dom.querySelector('.ant-table-body');
    const target = dom.querySelector('.ant-table-body table');
    return [box, target];
};

function ResizeTable<T extends object>(prop: Iprop<T>): JSX.Element {
    const { columns: propColumns, variableTotalWidth = false, minThWidth = 50, id, style, virtual = false, scroll, dataSource, ...restProps } = prop;
    const [columns, setColumns] = useState<ColumnsType<T>>([]);

    // ============================ Resize ============================
    useEffect(() => {
        setColumns(propColumns ?? []);
    }, [JSON.stringify(propColumns)]);
    const mergeColumns: any = useMemo(() => columns.map((col, index) => ({
        ...col,
        onHeaderCell: () => ({
            onResize: (deltaX: number, width: number, nextWidth?: number): void => {
                const newColumns = getResizeColumns({ columns, index, width, nextWidth, minThWidth, variableTotalWidth });
                setColumns(newColumns);
            },
            resizable: variableTotalWidth || index !== propColumns.length - 1,
        }),
        // ============================ filters ============================
        ...((isArray(col.filters) && col.filters.length > 0) ? getColumnSearchProps() : {}),
    })), [columns]);

    // ============================ virtual ============================
    const ref = useRef(null);
    const { data: renderList, boxRef, targetRef } = useWatchVirtualRender(
        { visibleHeight: scroll?.y ?? 0, itemHeight: scroll?.rowHeight ?? 39, dataSource });
    useEffect(() => {
        if (!virtual || ref.current === null) {
            return;
        }
        const [box, target] = getVirtualElement(ref.current as Element);
        if (box !== null && target !== null) {
            (boxRef as any).current = box;
            (targetRef as any).current = target;
        }
    }, []);

    // 出现分页跳转输入框
    useEffect(() => {
        if (prop.pagination?.showSizeChanger) {
            limitInput();
        }
    }, [prop.pagination]);

    return (
        <div id={id} style={{ ...style ?? {} }} ref={ref}>
            <Table
                { ...restProps }
                scroll={scroll}
                dataSource={virtual ? renderList : dataSource}
                className={!variableTotalWidth ? '' : 'variableTotalWidth'}
                columns={mergeColumns}
                components={{ header: { cell: resizableTitle } }}
            />
        </div>
    );
};

export default ResizeTable;
