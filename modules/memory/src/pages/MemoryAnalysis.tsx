/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

import Grid from '@material-ui/core/Grid';
import { makeStyles } from '@material-ui/core/styles';
import React, { useState, useEffect } from 'react';
import { observer } from 'mobx-react-lite';
import { AntTableChart } from '../components/AntTableChart';
import { LineChart } from '../components/LineChart';
import { Button, Input, InputNumber, message, Select, Spin } from 'antd';
import { Session } from '../entity/session';
import { Graph, MemoryCurve, MemoryTableColumn, OperatorDetail, OperatorMemoryCondition } from '../utils/interface';
import { memoryCurveGet, operatorsMemoryGet } from '../utils/RequestUtils';
import { Label } from '../components/Common';
import i18n from '../i18n';

interface SelectedRange {
    startTs: number;
    endTs: number;
}

const useStyles = makeStyles((theme) => ({
    topFilter: {
        height: '60px',
        lineHeight: '60px',
    },
    middleFilter: {
        height: '60px',
        lineHeight: '60px',
    },
}));

const lineColumn = [ 'Time (ms)', 'Operators Allocated (MB)', 'Operators Reserved (MB)' ];
const tableColumn: MemoryTableColumn[] = [ { name: 'Name', type: 'string', key: 'name' },
    { name: 'Size(KB)', type: 'number', key: 'size' },
    { name: 'Allocation Time(ms)', type: 'number', key: 'allocationTime' },
    { name: 'Release Time(ms)', type: 'number', key: 'releaseTime' },
    { name: 'Duration(ms)', type: 'number', key: 'duration' } ];

// eslint-disable-next-line max-lines-per-function
const MemoryAnalysis = observer(function({ session, isDark }: { session: Session; isDark: boolean }) {
    const classes = useStyles();
    // 算子表格内存信息
    const [ memoryTableData, setMemoryTableData ] = useState<OperatorDetail[]>([]);
    // 内存曲线数据源
    const [ memoryCurveData, setMemoryCurveData ] = useState<MemoryCurve | undefined>(undefined);
    // 内存曲线绘制数据
    const [ lineChartData, setLineChartData ] = useState<Graph | undefined>(undefined);
    const [ selectedRange, setSelectedRange ] = useState<SelectedRange | undefined>();
    const [ searchEventOperatorName, setSearchEventOperatorName ] = useState<string>('');
    const [ minSize, setMinSize ] = useState<number>(0);
    // 最大内存范围，默认1000000KB
    const [ maxSize, setMaxSize ] = useState<number>(1000000);
    const [ curveSpin, setCurveSpin ] = useState<boolean>(false);
    const [ tableSpin, setTableSpin ] = useState<boolean>(false);
    const [ rankId, setRankId ] = useState<string | undefined>(undefined);
    const [ rankIdList, setRankIdList ] = useState<string[]>([]);
    const [ current, setCurrent ] = useState<number>(1);
    const [ pageSize, setPageSize ] = useState<number>(10);
    const [ total, setTotal ] = useState<number>(0);
    const [ orderBy, setOrderBy ] = useState<string>('name');

    const onSearchEventOperatorChanged: React.ChangeEventHandler<HTMLInputElement | HTMLTextAreaElement> = (event) => {
        setSearchEventOperatorName(event.target.value as string);
    };

    const [ selectedRecord, setSelectedRecord ] = useState<any | undefined>();
    const onRowSelected = (record?: object, rowIndex?: number): void => {
        setSelectedRecord(record);
    };

    const onFilterEventMinSizeInputChanged = (value: number | null): void => {
        setMinSize(value as number);
    };

    const onFilterEventMaxSizeInputChanged = (value: number | null): void => {
        setMaxSize(value as number);
    };

    const onSearch = (orderName: string, minSize: number, maxSize: number): void => {
        if (rankId === undefined) {
            return;
        }
        if (maxSize < minSize) {
            message.warning(i18n.t('Invalid Size Warning'));
            return;
        }
        const param: OperatorMemoryCondition = {
            rankId,
            token: session.token,
            currentPage: current,
            pageSize,
            orderBy,
            orderName,
            minSize,
            maxSize,
        };
        if (selectedRange) {
            param.startTime = selectedRange.startTs;
            param.endTime = selectedRange.endTs;
        }

        setTableSpin(true);
        operatorsMemoryGet(param).then((resp) => {
            const operatorDetails = resp.operatorDetail;
            setTotal(resp.totalNum);
            setMemoryTableData(operatorDetails);
            setTableSpin(false);
        }).catch(err => {
            message.error(err);
        });
    };

    const onRankIdChanged = (value: string): void => {
        setRankId(value);
        setSelectedRange(undefined);
        setSearchEventOperatorName('');
        setCurrent(1);
        setPageSize(10);
        setMinSize(0);
        setMaxSize(1000000);
    };

    const onReset = (): void => {
        setSearchEventOperatorName('');
        setMinSize(0);
        setMaxSize(1000000);
        onSearch('', 0, 1000000);
    };

    const onSelectedRangeChanged = (start: number, end: number): void => {
        if (start > end || !memoryCurveData) {
            setSelectedRange(undefined);
            return;
        }

        const allDatas = memoryCurveData.AllocatesLine;
        if (allDatas.length <= 1) {
            setSelectedRange(undefined);
            return;
        }

        setSelectedRange({ startTs: allDatas[start][0], endTs: allDatas[end][0] });
        setCurrent(1);
        setPageSize(10);
    };

    useEffect(() => {
        onSearch(searchEventOperatorName, minSize, maxSize);
    }, [ selectedRange, rankId, current, pageSize ]);

    useEffect(() => {
        if (rankId === undefined) {
            return;
        }

        setCurveSpin(true);
        memoryCurveGet({ rankId, token: session.token }).then((resp) => {
            // Reset the select range to null when rankId changes
            setSelectedRange(undefined);
            setMemoryCurveData(resp);
            setLineChartData({
                title: resp.peakMemoryUsage,
                columns: resp.AppLine.length > 0 ? [ ...lineColumn, 'APP Reserved' ] : lineColumn,
                rows: {
                    allocated: resp.AllocatesLine,
                    reserved: resp.ReservedLine,
                    app: resp.AppLine,
                },
            });
            setCurveSpin(false);
        }).catch(err => {
            message.error(err);
        });
    }, [rankId]);

    useEffect(() => {
        setRankIdList(session.memoryRankIds);
    }, [session.memoryRankIds]);

    return (
        <div className="memory-analysis-wrapper">
            <Grid direction="column" container spacing={1}>
                <Grid item>
                    <Grid container direction="column">
                        <Grid container>
                            <Grid item className={classes.topFilter}>
                                <Label name="RankId" />
                                <Select
                                    value={rankId}
                                    style={{ width: 200 }}
                                    onChange={onRankIdChanged}
                                    options={rankIdList.map((rankId) => {
                                        return {
                                            value: rankId,
                                            label: rankId,
                                        };
                                    })}
                                />
                            </Grid>
                        </Grid>
                        <Grid item>
                            <Spin spinning={curveSpin} tip="loading...">
                                { lineChartData &&
                                    <LineChart
                                        hAxisTitle="Time (ms)"
                                        vAxisTitle="Memory Usage (MB)"
                                        graph={lineChartData}
                                        onSelectionChanged={onSelectedRangeChanged}
                                        record={selectedRecord}
                                        isDark={isDark}
                                    />
                                }
                            </Spin>
                        </Grid>
                    </Grid>
                </Grid>
                <Grid container className={classes.middleFilter}>
                    <Grid item container sm={3}>
                        <Grid item>
                            <Label name={i18n.t('Name')} />
                            <Input
                                value={searchEventOperatorName}
                                style={{ width: 200 }}
                                onChange={onSearchEventOperatorChanged}
                                placeholder="Search by Name"
                                allowClear
                                maxLength={200}
                            />
                        </Grid>
                    </Grid>
                    <Grid item container sm={3}>
                        <Grid item>
                            <Label name={i18n.t('Min Size')} />
                            <InputNumber
                                value={minSize}
                                style={{ width: 200 }}
                                onChange={onFilterEventMinSizeInputChanged}
                                min={0}
                                formatter={value => `${Number(value)}`}
                            />
                        </Grid>
                    </Grid>
                    <Grid item container sm={3}>
                        <Grid item>
                            <Label name={i18n.t('Max Size')} />
                            <InputNumber
                                value={maxSize}
                                style={{ width: 200 }}
                                onChange={onFilterEventMaxSizeInputChanged}
                                min={0}
                                minLength={1}
                                formatter={value => `${Number(value)}`}
                            />
                        </Grid>
                    </Grid>
                    <Grid item container sm={3}>
                        <Grid item>
                            <Button
                                onClick={() => onSearch(searchEventOperatorName, minSize, maxSize)}
                                type="primary"
                                style={{ marginRight: 10, width: 100 }}
                                disabled={memoryTableData.length <= 0}
                            >
                                {i18n.t('Button Query')}
                            </Button>
                            <Button
                                onClick={onReset}
                                style={{ width: 100 }}
                                disabled={memoryTableData.length <= 0}
                            >
                                {i18n.t('Button Reset')}
                            </Button>
                        </Grid>
                    </Grid>
                </Grid>
                <Grid item>
                    <Spin spinning={tableSpin} tip="loading...">
                        <AntTableChart
                            tableData={{
                                columns: tableColumn,
                                rows: memoryTableData,
                            }}
                            onRowSelected={onRowSelected}
                            current={current}
                            pageSize={pageSize}
                            onCurrentChange={setCurrent}
                            onPageSizeChange={setPageSize}
                            total={total}
                        />
                    </Spin>
                </Grid>
            </Grid>
        </div>
    );
});
export default MemoryAnalysis;
