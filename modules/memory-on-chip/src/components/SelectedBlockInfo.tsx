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

import React, { useMemo } from 'react';
import { observer } from 'mobx-react-lite';
import styled from '@emotion/styled';
import { useTranslation } from 'react-i18next';
import { MIDescriptions, MIDescriptionsItem, CollapsiblePanel } from '@insight/lib/components';
import { useStores } from '@/stores';

const InfoContainer = styled.div`
    background-color: ${({ theme }) => theme.bgColorLighter};
    border-radius: ${({ theme }) => theme.borderRadiusBase};
    border: 1px solid ${({ theme }) => theme.borderColorLight};
`;

const AttrBox = styled.div`
    background-color: ${({ theme }) => theme.bgColorLight};
    padding: 12px;
    border-radius: ${({ theme }) => theme.borderRadiusSmall};
    font-family: monospace;
    font-size: 12px;
    color: ${({ theme }) => theme.textColorSecondary};
    white-space: pre-wrap;
    word-break: break-all;
    max-height: 200px;
    overflow-y: auto;
    margin-top: 8px;
`;

export const SelectedBlockInfo: React.FC = observer(() => {
    const { t } = useTranslation('triton');
    const { tritonStore } = useStores();
    const { selectedBlock } = tritonStore;

    const parsedAttr = useMemo(() => {
        if (!selectedBlock?.attr) return null;
        try {
            return JSON.parse(selectedBlock.attr);
        } catch (e) {
            return selectedBlock.attr;
        }
    }, [selectedBlock]);

    if (!selectedBlock) return null;

    return (
        <InfoContainer>
            <CollapsiblePanel title={t('sliceDetail')} defaultOpen={true}>
                <MIDescriptions style={{ gap: '32px' }}>
                    <MIDescriptionsItem label="ID">{selectedBlock.id}</MIDescriptionsItem>
                    <MIDescriptionsItem label="Address">{(selectedBlock.addr / 1024).toFixed(2)} KB</MIDescriptionsItem>
                    <MIDescriptionsItem label="Size">{(selectedBlock.size / 1024).toFixed(2)} KB</MIDescriptionsItem>
                    <MIDescriptionsItem label="Life">[{selectedBlock._startTimestamp}, {selectedBlock._endTimestamp}]</MIDescriptionsItem>
                </MIDescriptions>

                <div style={{ marginTop: '16px' }}>
                    <strong>Attributes:</strong>
                    <AttrBox>
                        {typeof parsedAttr === 'object'
                            ? JSON.stringify(parsedAttr, null, 2)
                            : parsedAttr}
                    </AttrBox>
                </div>
            </CollapsiblePanel>
        </InfoContainer>
    );
});
