/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import React, { useEffect, useMemo, useRef, useState } from 'react';
import { observer } from 'mobx-react-lite';
import { Legend, MIChart } from 'ascend-components';
import type { ChartsHandle } from 'ascend-components/MIChart';
import type { EChartsOption, CustomSeriesRenderItem } from 'echarts';
import { merge } from 'lodash';
import { type Theme, useTheme } from '@emotion/react';
import { clamp, colorPalette, safeStr, StyledEmpty } from 'ascend-utils';
import { useStores } from '@/stores';
import { GetTraceDataResults } from '@/api/types';

const TEXT_PADDING = 4;
const TEXT_PADDING_X = TEXT_PADDING * 2;
const RANK_HEIGHT = 50;
const CHART_BASE_HEIGHT = 200;

type Ranks = string[];
interface SeriesDataItem {
    name: string;
    value: Array<string | number>;
    itemStyle: {
        color: string;
    };
}

const PREFIX = 'prefix_';

const addPrefix = (str: string, prefix = PREFIX): string => {
    return prefix + str;
};

const removePrefix = (str: string, prefix = PREFIX): string => {
    if (str.startsWith(prefix)) {
        return str.slice(prefix.length);
    }
    return str;
};

const nsToMs = (ns: number): number => {
    return ns / 1000000;
};

// Host 机器别名映射（由于长度原因，y轴刻度显示别名）
const hostAliasMap: Map<string, string> = new Map();

const colorMap = new Map();
const hashToNumber = (name: string): number => {
    if (!colorMap.has(name)) {
        colorMap.set(name, colorMap.size);
    }

    return (colorMap.get(name) ?? 0) % colorPalette.length;
};

const splitLabelByRank = (str: string): { before: string; after: string } | null => {
    const match = str.match(/^(.*?)( Rank )(.*)$/);
    if (match) {
        return {
            before: match[1],
            after: match[3],
        };
    }
    return null;
};

//  指定维度的映射值
const valueMap = {
    hostRank: 0, // host+rank
    start: 1,
    end: 2,
    duration: 3,
    fileId: 4,
    nodeType: 5,
    stageType: 6,
    host: 7,
    rankId: 8,
    name: 9,
};

const formatData = (dataSource: GetTraceDataResults, theme: Theme): {ranks: Ranks; data: SeriesDataItem[]} => {
    const { taskData: rawData } = dataSource;
    const data: SeriesDataItem[] = [];
    const ranks = rawData.map(item => `${item.hostName} Rank ${item.rankId}`) ?? [];

    for (const item of rawData) {
        const { rankId, hostName } = item;
        const yName = `${hostName} Rank ${rankId}`;

        if (!hostAliasMap.get(item.hostName)) {
            hostAliasMap.set(item.hostName, `Host ${hostAliasMap.size + 1}`);
        }

        for (const slice of item.lists) {
            const { name, fileId, nodeType, stageType, startTime, duration } = slice;
            // 由于value中name的值可能为数字类型的字符串‘0’，导致echarts取值时会转为number类型，导致其他字符串类型的值变为NaN,所以加前缀处理，防止echarts将其转为数字类型
            data.push({
                name,
                value: [yName, nsToMs(startTime), nsToMs((startTime + duration)), nsToMs(duration), fileId, nodeType, stageType, item.hostName, item.rankId, addPrefix(name)],
                itemStyle: {
                    color: theme.colorPalette[colorPalette[hashToNumber(stageType)]],
                },
            });
        }
    }

    return { ranks, data };
};

const renderRect: CustomSeriesRenderItem = (_, api) => {
    const { start: vStart, end: vEnd, name: vName } = valueMap;
    const categoryIndex = api.value(0);
    const name = removePrefix(api.value(vName) as string);
    const start = api.coord([api.value(vStart), categoryIndex]);
    const end = api.coord([api.value(vEnd), categoryIndex]);
    const gridItemHeight = (api.size?.([0, 1]) as number[])[1];
    const rectWidth = end[0] - start[0];
    const rectHeight = gridItemHeight * 0.6;
    const textWidth = rectWidth > TEXT_PADDING_X ? Math.floor(rectWidth - TEXT_PADDING_X) : rectWidth;

    return {
        type: 'rect',
        transition: ['shape'],
        shape: {
            x: start[0],
            y: start[1] - rectHeight / 2,
            width: rectWidth,
            height: rectHeight,
        },
        textContent: {
            type: 'text',
            invisible: rectHeight < 12,
            style: {
                text: name,
                fill: '#ffffff',
                overflow: 'truncate',
                width: textWidth,
            },
        },
        textConfig: {
            position: 'inside',
            inside: true,
            local: true,
        },
        style: {
            fill: api.visual('color'),
        },
    };
};

const baseOptions: EChartsOption = {
    grid: {
        left: 100,
        right: 100,
    },
    tooltip: {
        formatter: function (params: any): string {
            return `
            <div class="formatter">
                <div class="row marker">${params.marker}</div>
                 <div class="row">
                    <div class="label">Name</div>
                    <div class="value">${safeStr(params.value[valueMap.name])}</div>
                </div>
                <div class="row">
                    <div class="label">Node Type</div>
                    <div class="value">${safeStr(params.value[valueMap.nodeType])}</div>
                </div>
                <div class="row">
                    <div class="label">Stage Type</div>
                    <div class="value">${safeStr(params.value[valueMap.stageType])}</div>
                </div>
                <div class="row">
                    <div class="label">Host</div>
                    <div class="value">${safeStr(params.value[valueMap.host])}</div>
                </div>
                <div class="row">
                    <div class="label">Rank</div>
                    <div class="value">${safeStr(params.value[valueMap.rankId])}</div>
                </div>
                <div class="row">
                    <div class="label">Start Time</div>
                    <div class="value">${safeStr(params.value[valueMap.start])} ms</div>
                </div>
                <div class="row">
                    <div class="label">Duration</div>
                    <div class="value">${safeStr(params.value[valueMap.duration])} ms</div>
                </div>
            </div>
            `;
        },
    },
    dataZoom: [
        {
            type: 'slider',
            xAxisIndex: 0,
            filterMode: 'weakFilter',
            bottom: 10,
            height: 20,
            labelFormatter: '',
            borderColor: '#d2dbee80',
        },
        {
            type: 'slider',
            yAxisIndex: 0,
            filterMode: 'weakFilter',
            right: 10,
            width: 20,
            labelFormatter: '',
            borderColor: '#d2dbee80',
        },
        {
            type: 'inside',
            filterMode: 'weakFilter',
            zoomOnMouseWheel: 'ctrl',
            moveOnMouseMove: 'ctrl',
            moveOnMouseWheel: 'shift',
        },
    ],
    xAxis: {
        name: 'Time(ms)',
        scale: true,
        axisLabel: {
            formatter (value: number): string {
                const ms = Math.floor(value / 1000);
                return `${ms} k`;
            },
        },
    },
    yAxis: {
        name: 'Host Alias + Rank',
        axisLabel: {
            formatter (value: string): string {
                const result = splitLabelByRank(value);
                const host = result?.before ? `${hostAliasMap.get(result.before)}` : '';
                const rank = result?.after ? `Rank ${result?.after}` : '';
                return `${host} ${rank}`;
            },
        },
    },
    series: [
        {
            type: 'custom',
            renderItem: renderRect,
            itemStyle: {
                opacity: 0.8,
            },
            encode: {
                x: [1, 2],
                y: 0,
            },
            clip: true,
        },
    ],
};

export const TraceTimeline = observer((): JSX.Element => {
    const { traceStore, sessionStore } = useStores();
    const { parseCompleted } = sessionStore;
    const { loading, traceData, stageTypeList, getTraceData } = traceStore;
    const chartRef = useRef<ChartsHandle>(null);
    const [chartOptions, setChartOptions] = useState<EChartsOption>({});
    const theme = useTheme();
    const [chartHeight, setChartHeight] = useState('400px');
    const legendItems = useMemo(() => {
        return stageTypeList.map(
            (stageType) => ({
                label: stageType,
                color: theme.colorPalette[colorPalette[hashToNumber(stageType)]],
            }),
        );
    }, [stageTypeList]);
    const syncScroll = (e: WheelEvent): void => {
        if ((e.target as HTMLElement).tagName !== 'CANVAS') {
            return;
        }
        if (!e.ctrlKey && !e.shiftKey) {
            document.querySelector('.mi-page-content')?.scrollBy(0, e.deltaY);
        }
    };

    useEffect(() => {
        if (parseCompleted) {
            getTraceData();
        }
    }, [parseCompleted]);

    useEffect(() => {
        if (traceData === null) {
            setChartOptions({});
            return;
        }
        hostAliasMap.clear();

        const { data, ranks } = formatData(traceData, theme);
        const height = Math.round(clamp((ranks.length * RANK_HEIGHT) + CHART_BASE_HEIGHT, 200, 800));

        setChartHeight(`${height}px`);
        setChartOptions(merge({}, baseOptions, {
            yAxis: {
                data: ranks,
            },
            series: [
                { data },
            ],
        }));
    }, [traceData]);

    // echarts配置项中设置zoomOnMouseWheel: 'ctrl' 后, 滚轮在图表上无法触发滚动，此处需要手动处理滚动
    useEffect(() => {
        const chartDom = chartRef.current?.getChartDom();

        chartDom?.addEventListener('wheel', syncScroll, true);

        return (): void => {
            chartDom?.removeEventListener('wheel', syncScroll, true);
        };
    }, []);

    return traceData
        ? <div>
            <div className="mb-20">
                <Legend items={legendItems}></Legend>
            </div>
            <MIChart
                ref={chartRef}
                width={'calc(100vw - 80px)'}
                height={chartHeight}
                loading={loading}
                options={chartOptions}
            />
        </div>
        : <StyledEmpty></StyledEmpty>
    ;
});
