/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import React, { useEffect, useState } from 'react';
import { type IblockData } from './Index';
// eslint-disable-next-line import/no-unresolved
import ResizeTable from 'lib/ResizeTable';
// eslint-disable-next-line import/no-unresolved
import { getSet, firstLetterUpper } from 'lib/CommonUtils';

interface Iprops {
    blockId: string;
    data: IblockData[];
}

interface Iobj {
    [x: string]: string;
}

interface ItableConfig {
    blockType: string;
    cols: any[];
    dataset: Iobj[];
}

function getFullCols(blockType: string, blockTypeData: IblockData[]): any[] {
    const firstCol = {
        title: blockType?.toUpperCase(),
        dataIndex: 'name',
        ellipsis: true,

    };
    const units = getSet(blockTypeData, 'unit') as string[];
    const restCols = units.map((item, index) => (
        {
            title: firstLetterUpper(item),
            dataIndex: item,
            ellipsis: true,
        }
    ));
    return [firstCol, ...restCols];
}

function Index({ blockId, data }: Iprops): JSX.Element {
    const [tablelist, setTablelist] = useState<ItableConfig[]>([]);

    const updateTable = (): void => {
        const showData = data.filter(item => item.blockId === blockId);
        // 按照blockType分表
        const blockTypeSet = getSet(showData, 'blockType') as string[];
        const dataGroupByBlockType = blockTypeSet.map(blockType => {
            const blockTypeData = showData.filter(item => item.blockType === blockType);
            const cols = getFullCols(blockType, blockTypeData);
            const dataObj: Record<string, Iobj> = {};
            blockTypeData.forEach(item => {
                if (dataObj[item.name] === undefined) {
                    dataObj[item.name] = { name: item.name };
                }
                dataObj[item.name][item.unit] = item.value;
            });
            const dataset = Object.keys(dataObj).map(name => dataObj[name]);
            return { blockType, cols, dataset };
        });
        setTablelist(dataGroupByBlockType);
    };
    useEffect(() => {
        updateTable();
    }, [blockId, data]);
    return (
        <div style={{ padding: '0 20px' }}>
            {tablelist.length === 0 &&
                (<div style={{ textAlign: 'center', color: 'var(--grey15) ' }}>No data</div>)
            }
            {
                tablelist.map(item => (
                    <ResizeTable
                        key={item.blockType}
                        size="small"
                        columns={item.cols ?? []}
                        dataSource={item.dataset ?? []}
                        scroll={item.dataset.length > 10 ? { y: 500 } : false}
                        pagination={{ defaultPageSize: item.dataset.length, showSizeChanger: false, hideOnSinglePage: true }}
                    />
                ))
            }
        </div>
    );
}

export default Index;
