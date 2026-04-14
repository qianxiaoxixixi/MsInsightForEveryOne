/*
 * -------------------------------------------------------------------------
 * This file is part of the MindStudio project.
 * Copyright (c) 2026 Huawei Technologies Co.,Ltd.
 *
 * MindStudio is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v2 for more details.
 * -------------------------------------------------------------------------
 */

import React, { useMemo, useCallback, useRef, useEffect } from 'react';
import { observer } from 'mobx-react-lite';
import { useTheme } from '@emotion/react';
import { MIChart } from '@insight/lib/components';
import { colorPalette } from '@insight/lib/utils';
import { useStores } from '@/stores';

interface MemoryLifeChartProps {
    onBlockClick?: (timestamp: number, blockId?: string, addr?: number) => void;
}

export const MemoryLifeChart: React.FC<MemoryLifeChartProps> = observer(({ onBlockClick }) => {
    const { tritonStore } = useStores();
    const theme = useTheme();
    const chartRef = useRef<any>(null);

    const renderItem = useCallback((_params: any, api: any): any => {
        const startTimestamp = api.value(0);
        const endTimestamp = api.value(1);
        const yStart = api.value(2);
        const size = api.value(3);
        const color = api.value(7);

        const start = api.coord([startTimestamp, yStart]);
        // Use a small constant for time delta if it's 0 to ensure size calculation works
        const timeDelta = Math.max(endTimestamp - startTimestamp, 0.001);
        const sizePx = api.size([timeDelta, size]);

        // Ensure minimum visibility
        const width = Math.max(sizePx[0], 2);
        const height = Math.max(sizePx[1], 4);

        return {
            type: 'rect',
            shape: {
                x: start[0],
                y: start[1] - height,
                width,
                height,
            },
            style: api.style({
                fill: color,
                lineWidth: 1,
                stroke: 'rgba(0,0,0,0.3)',
            }),
        };
    }, []);

    // Map each id to a color from the palette
    const options = useMemo(() => {
        const blocks = tritonStore.processedBlocks;
        const uniqueIds = Array.from(new Set(blocks.map(b => b.id)));

        const data = blocks.map((block, index) => {
            const idIndex = uniqueIds.indexOf(block.id);
            const colorKey = colorPalette[idIndex % colorPalette.length];
            const color = (theme as any).colorPalette?.[colorKey] || '#ccc';

            return {
                // Provide a unique name/id for each data item to prevent ECharts from merging them
                name: `block-${block.id}-${block.addr}-${index}`,
                value: [
                    block._startTimestamp,
                    block._endTimestamp,
                    block.addr, // yStart
                    block.size,
                    block.addr,
                    block.id,
                    Number(block.addr + block.size), // yEnd
                    color,
                ],
            };
        });

        return {
            tooltip: {
                trigger: 'item',
                axisPointer: {
                    type: 'cross',
                    label: {
                        padding: [4, 8, 4, 8],
                        formatter: (params: any) => {
                            if (params.axisDimension === 'x') {
                                return `Time: ${Math.round(params.value)}`;
                            }
                            const value = params.value;
                            if (value >= 1024 * 1024) return `${(value / (1024 * 1024)).toFixed(1)} MB`;
                            if (value >= 1024) return `${(value / 1024).toFixed(1)} KB`;
                            return `${Math.round(value)} B`;
                        },
                    },
                },
                formatter: (params: any) => {
                    const [start, end, , size, addr, id] = params.value;
                    return `
                        ID: ${id}<br/>
                        Address: ${(addr / 1024).toFixed(2)} KB<br/>
                        Size: ${(size / 1024).toFixed(2)} KB<br/>
                        Life: [${start}, ${end}]
                    `;
                },
            },
            grid: {
                top: 60,
                bottom: 80,
                left: 100,
                right: 80,
            },
            xAxis: {
                type: 'value',
                name: 'Buffer Life',
                nameLocation: 'middle',
                nameGap: 40,
                scale: true,
                splitLine: { show: true },
            },
            yAxis: {
                type: 'value',
                name: 'Buffer Size',
                scale: true,
                axisLabel: {
                    formatter: (value: number) => {
                        if (value >= 1024 * 1024) return `${(value / (1024 * 1024)).toFixed(1)} MB`;
                        if (value >= 1024) return `${(value / 1024).toFixed(1)} KB`;
                        return `${value} B`;
                    },
                },
            },
            series: [{
                type: 'custom',
                renderItem,
                // Rename ID to BlockID to avoid ECharts treating it as a unique key for deduplication
                dimensions: ['Start', 'End', 'YStart', 'Size', 'Addr', 'BlockID', 'YEnd', 'Color'],
                encode: {
                    x: [0, 1],
                    y: [2, 6],
                    tooltip: [0, 1, 3, 4],
                },
                data,
            }],
        };
    }, [tritonStore.processedBlocks, theme, renderItem]);

    useEffect(() => {
        const chart = chartRef.current?.getInstance();
        if (!chart) return;

        const handleZrClick = (params: any): void => {
            // Only trigger for blank space clicks. If a block is clicked, the regular 'click' event handles it.
            if (!params.target && onBlockClick) {
                const pointInPixel = [params.offsetX, params.offsetY];
                if (chart.containPixel('grid', pointInPixel)) {
                    const pointInGrid = chart.convertFromPixel('grid', pointInPixel);
                    onBlockClick(pointInGrid[0]);
                }
            }
        };

        chart.getZr().on('click', handleZrClick);
        return (): void => {
            chart.getZr()?.off('click', handleZrClick);
        };
    }, [onBlockClick, tritonStore.loadingLife]);

    const handleEvents = useMemo(() => ({
        click: (params: any): void => {
            if (params.data && onBlockClick) {
                const chart = chartRef.current?.getInstance();
                let clickedTimestamp = params.data.value[0];
                if (chart && params.event) {
                    const pointInPixel = [params.event.offsetX, params.event.offsetY];
                    const pointInGrid = chart.convertFromPixel('grid', pointInPixel);
                    if (pointInGrid) {
                        clickedTimestamp = pointInGrid[0];
                    }
                }
                // Return EXACT clicked timestamp, block ID, and addr
                onBlockClick(clickedTimestamp, params.data.value[5], params.data.value[4]);
            }
        },
    }), [onBlockClick]);

    return (
        <MIChart
            ref={chartRef}
            options={options as any}
            loading={tritonStore.loadingLife}
            height="500px"
            onEvents={handleEvents}
        />
    );
});
