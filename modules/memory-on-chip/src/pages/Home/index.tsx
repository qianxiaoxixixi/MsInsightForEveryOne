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

import React, { useEffect } from 'react';
import { observer } from 'mobx-react-lite';
import { useTranslation } from 'react-i18next';
import { CollapsiblePanel, Select } from '@insight/lib/components';
import { useStores } from '@/stores';
import { MemoryLifeChart } from '@/components/MemoryLifeChart';
import { SelectedBlockInfo } from '@/components/SelectedBlockInfo';
import { MemoryLayoutChart } from '@/components/MemoryLayoutChart';
import {
    ChartWrapper, EmptyTipsWrapper, HeaderContainer, TitleContainer, SelectContainer,
    CurrentTimeContainer, ErrorBanner, StatusBadge,
} from './styles';

export const Home: React.FC = observer(() => {
    const { t } = useTranslation('triton');
    const { tritonStore } = useStores();

    useEffect(() => {
        tritonStore.fetchBasicInfo();
    }, [tritonStore]);

    const handleBlockClick = React.useCallback((timestamp: number, blockId?: string, addr?: number): void => {
        const roundedTimestamp = Math.floor(timestamp);
        tritonStore.setSelectedTimestamp(roundedTimestamp);
        tritonStore.fetchMemoryUsage(roundedTimestamp);

        // Find and set selected block for detail display using both id and addr
        if (blockId !== undefined && addr !== undefined) {
            const block = tritonStore.processedBlocks.find(b => b.id === blockId && b.addr === addr);
            if (block) {
                tritonStore.setSelectedBlock(block);
            } else {
                tritonStore.setSelectedBlock(null);
            }
        } else {
            tritonStore.setSelectedBlock(null);
        }
    }, [tritonStore]);

    const handleMemTypeChange = (value: string): void => {
        tritonStore.setMemType(value);
    };

    const memoryStatus = tritonStore.memoryLifeData?.status;
    const hasError = memoryStatus === 'fail';
    const errMsg = tritonStore.memoryLifeData?.errMsg;

    return (
        <div>
            {tritonStore.basicInfo && (
                <HeaderContainer>
                    <TitleContainer>
                        {t('Kernel Name')}: {tritonStore.basicInfo.kernelName}
                    </TitleContainer>
                    {(
                        <SelectContainer>
                            <span>{t('Scope Type')}:</span>
                            <Select
                                value={tritonStore.selectedMemType}
                                onChange={handleMemTypeChange}
                                style={{ width: 120 }}
                                options={tritonStore.basicInfo.scopeTypes?.map(type => ({ label: type, value: type }))}
                            />
                            {memoryStatus !== undefined && (
                                <StatusBadge status={hasError ? 'error' : 'success'}>
                                    {hasError ? t('Compile Failed') : t('Compile Success')}
                                </StatusBadge>
                            )}
                        </SelectContainer>
                    )}
                </HeaderContainer>
            )}

            {errMsg && (
                <ErrorBanner>
                    <strong>{t('Error Info')}:</strong>
                    <br />
                    {errMsg}
                </ErrorBanner>
            )}

            <CollapsiblePanel title={t('MemoryBlocks')}>
                <ChartWrapper>
                    <MemoryLifeChart onBlockClick={handleBlockClick} />
                </ChartWrapper>
                <SelectedBlockInfo />
            </CollapsiblePanel>

            <CollapsiblePanel title={t('Memory Layout')} defaultOpen={true}>
                {tritonStore.selectedTimestamp !== null
                    ? (
                        <>
                            <CurrentTimeContainer>
                                <h3>{t('Current Time')}: {tritonStore.selectedTimestamp}</h3>
                            </CurrentTimeContainer>
                            <MemoryLayoutChart />
                        </>
                    )
                    : (
                        <EmptyTipsWrapper>
                            {t('EmptyTips')}
                        </EmptyTipsWrapper>
                    )}
            </CollapsiblePanel>
        </div>
    );
});
