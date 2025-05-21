/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { ExpertLoadBalancingForm } from './Form';
import { ExpertLoadBalancingChart } from './Chart';
import React, { useCallback, useEffect, useState } from 'react';
import connector from '../../../connection';
import { importExpertData, queryModelInfo, queryExpertHotspot } from '../../../utils/RequestUtils';
import { QueryExpertHotspotItem } from '../../../utils/interface';
import { message } from 'antd';
import { Session } from '../../../entity/session';

export interface FormData {
    layerNum: number | null;
    denseLayerList: number[];
    expertNum: number | null;
    modelStage: 'prefill' | 'decode';
    version: 'unbalanced' | 'balanced';
}

export const ExpertLoadBalancingBox = ({ session }: { session: Session }): React.ReactElement => {
    const [formData, setFormData] = useState<FormData>({
        layerNum: null,
        denseLayerList: [],
        expertNum: null,
        modelStage: 'prefill',
        version: 'unbalanced',
    });
    const [chartData, setChartData] = useState<QueryExpertHotspotItem[]>([]);
    const [chartLoading, setChartLoading] = useState(false);

    // 请求图表热点数据
    const fetchChatData = async (params?: FormData): Promise<void> => {
        const { layerNum, expertNum, modelStage, version, denseLayerList } = params ?? formData;

        try {
            if (layerNum === null || expertNum === null) {
                setChartData([]);
                return;
            }
            setChartLoading(true);
            const { hotspotInfos } = await queryExpertHotspot({
                layerNum,
                expertNum,
                modelStage,
                version,
                denseLayerList,
            });

            setChartLoading(false);
            setChartData(hotspotInfos);
        } catch (error) {
            message.error((error as any)?.message);
            setChartLoading(false);
        }
    };

    // 监听确认导入数据动作，执行导入数据逻辑
    const handleExpertDataImport = useCallback(async (e: MessageEvent<{ event: string; body: Record<string, unknown> }>): Promise<void> => {
        const res = e.data;
        if (res.body === undefined || typeof res.body !== 'object') {
            return;
        }

        const filePath = res.body?.path as string ?? '';
        if (filePath === '') {
            return;
        }

        try {
            await importExpertData({
                version: formData.version,
                filePath,
            });

            const conditions = await getFormConditions();
            await fetchChatData(conditions);
        } catch (error) {
            message.error((error as any)?.message);
        }
    }, [formData]);

    // 查询搜索条件
    const getFormConditions = async (): Promise<FormData> => {
        const data = await queryModelInfo();
        const processedData = {
            ...data,
            layerNum: data.layerNum === 0 ? null : data.layerNum,
            expertNum: data.expertNum === 0 ? null : data.expertNum,
        };
        const newFormData = { ...formData, ...processedData };
        setFormData(newFormData);
        return newFormData;
    };

    // 处理表单数据的变化
    const handleFormChange = (changedValues: any, newFormData: FormData): void => {
        setFormData(newFormData);
        if (changedValues.version !== undefined || changedValues.modelStage !== undefined) {
            fetchChatData(newFormData);
        }
    };

    const handleFormSubmit = (): void => {
        fetchChatData();
    };

    const initLoad = async (): Promise<void> => {
        const conditions = await getFormConditions();
        await fetchChatData(conditions);
    };

    const handleImport = (): void => {
        connector.send({
            event: 'openImportDialog',
            keepRawData: true,
            body: {},
        });
    };

    useEffect(() => {
        const { sequence } = connector.addListener('importExpertLoadDataConfirm', handleExpertDataImport);

        return () => {
            connector.removeListener({ event: 'importExpertLoadDataConfirm', sequence });
        };
    }, [handleExpertDataImport]);

    useEffect(() => {
        initLoad();
    }, [session.renderId]);

    return <>
        <ExpertLoadBalancingForm
            loading={chartLoading}
            data={formData}
            onFormChange={handleFormChange}
            onFormSubmit={handleFormSubmit}
            onImport={handleImport}
        />
        <ExpertLoadBalancingChart
            loading={chartLoading}
            data={chartData}
        />
    </>;
};
