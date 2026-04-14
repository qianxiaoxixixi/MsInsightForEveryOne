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
import { useStores } from '@/stores';
import { colorPalette } from '@insight/lib/utils';
import { useTheme } from '@emotion/react';

const ScrollContainer = styled.div`
    width: 100%;
    overflow-x: auto;
    padding: 16px;
    background-color: ${({ theme }) => theme.bgColor};
`;

const GridContainer = styled.div`
    display: grid;
    grid-template-columns: repeat(16, minmax(60px, 1fr));
    gap: 8px;
    min-width: 1000px;
`;

const BankGroupHeader = styled.div`
    text-align: center;
    font-size: 12px;
    color: ${({ theme }) => theme.textColorPrimary};
    background-color: ${({ theme }) => theme.bgColorLight};
    padding: 4px;
    border-radius: 4px;
    font-weight: 500;
`;

const BankCell = styled.div`
    height: 140px;
    background-color: ${({ theme }) => theme.bgColorCommon};
    border: 1px solid ${({ theme }) => theme.borderColorLight};
    border-radius: 4px;
    position: relative;
    overflow: hidden;
    display: flex;
    justify-content: center;
    align-items: center;
`;

const BankLabel = styled.div`
    position: absolute;
    font-size: 10px;
    color: ${({ theme }) => theme.textColorPrimary};
    pointer-events: none;
    z-index: 10;
    text-align: center;
    opacity: 0.7;
`;

const ChunkElement = styled.div<{ bg: string; top: number; h: number; left: number; w: number }>`
    position: absolute;
    background-color: ${({ bg }) => bg};
    top: ${({ top }) => top}%;
    height: ${({ h }) => h}%;
    left: ${({ left }) => left}%;
    width: ${({ w }) => w}%;
    z-index: 2;
`;

interface ProcessedChunk {
    bankId: number;
    rowId: number;
    left: number;
    width: number;
    color: string;
}

const MEMORY_LAYOUT = {
    CHUNK_SIZE_BYTES: 32,
    CHUNKS_PER_MACRO_BLOCK: 2048,
    BANK_GROUPS: 16,
    MACRO_BLOCKS: 3,
    ROWS_PER_BANK: 128,
    TOTAL_BANKS: 48,
};

export const MemoryLayoutChart: React.FC = observer(() => {
    const { tritonStore } = useStores();
    const theme = useTheme();

    const { chunksByBank } = useMemo(() => {
        const usageData = tritonStore.currentUsageData;
        const lifeBlocks = tritonStore.memoryLifeData?.blocks ?? [];
        const uniqueIds = Array.from(new Set(lifeBlocks.map(b => b.id)));

        const chunks = new Map<number, ProcessedChunk[]>();

        // Initialize all banks
        for (let i = 0; i < MEMORY_LAYOUT.TOTAL_BANKS; i++) {
            chunks.set(i, []);
        }

        if (!usageData?.segments) {
            return { chunksByBank: chunks };
        }

        usageData.segments.forEach(segment => {
            segment.blocks.forEach((block) => {
                const startAddr = Number(block.addr);
                const size = block.size;
                const endAddr = startAddr + size - 1;

                const idIndex = uniqueIds.indexOf(block.id);
                const colorKey = colorPalette[idIndex >= 0 ? idIndex % colorPalette.length : 0];
                const color = (theme as any).colorPalette?.[colorKey] ?? '#ccc';

                const startChunkIndex = Math.floor(startAddr / MEMORY_LAYOUT.CHUNK_SIZE_BYTES);
                const endChunkIndex = Math.floor(endAddr / MEMORY_LAYOUT.CHUNK_SIZE_BYTES);

                for (let c = startChunkIndex; c <= endChunkIndex; c++) {
                    const macroBlockId = Math.floor(c / MEMORY_LAYOUT.CHUNKS_PER_MACRO_BLOCK);
                    const bankGroupId = c % MEMORY_LAYOUT.BANK_GROUPS;
                    const bankId = bankGroupId + (macroBlockId * MEMORY_LAYOUT.BANK_GROUPS);
                    const rowId = Math.floor((c % MEMORY_LAYOUT.CHUNKS_PER_MACRO_BLOCK) / MEMORY_LAYOUT.BANK_GROUPS);

                    // Physical limits safety check
                    if (bankId >= MEMORY_LAYOUT.TOTAL_BANKS || rowId >= MEMORY_LAYOUT.ROWS_PER_BANK) continue;

                    let left = 0;
                    let width = 100;

                    // Handle byte-level precision within the chunk for partial coverage
                    if (c === startChunkIndex && c === endChunkIndex) {
                        left = (startAddr % MEMORY_LAYOUT.CHUNK_SIZE_BYTES) / MEMORY_LAYOUT.CHUNK_SIZE_BYTES * 100;
                        width = (size / MEMORY_LAYOUT.CHUNK_SIZE_BYTES) * 100;
                    } else if (c === startChunkIndex) {
                        left = (startAddr % MEMORY_LAYOUT.CHUNK_SIZE_BYTES) / MEMORY_LAYOUT.CHUNK_SIZE_BYTES * 100;
                        width = 100 - left;
                    } else if (c === endChunkIndex) {
                        left = 0;
                        width = ((endAddr % MEMORY_LAYOUT.CHUNK_SIZE_BYTES) + 1) / MEMORY_LAYOUT.CHUNK_SIZE_BYTES * 100;
                    }

                    chunks.get(bankId)?.push({
                        bankId,
                        rowId,
                        left,
                        width,
                        color,
                    });
                }
            });
        });

        return { chunksByBank: chunks };
    }, [tritonStore.currentUsageData, tritonStore.memoryLifeData, theme]);

    const renderGrid = (): React.ReactNode[] => {
        const items: React.ReactNode[] = [];

        // 1. Render Headers
        for (let g = 0; g < MEMORY_LAYOUT.BANK_GROUPS; g++) {
            items.push(
                <BankGroupHeader key={`header-${g}`}>
                    Group {g}
                </BankGroupHeader>,
            );
        }

        // 2. Render Banks in rows (Macro Blocks)
        for (let r = 0; r < MEMORY_LAYOUT.MACRO_BLOCKS; r++) {
            for (let c = 0; c < MEMORY_LAYOUT.BANK_GROUPS; c++) {
                const bankId = r * MEMORY_LAYOUT.BANK_GROUPS + c;
                const bankChunks = chunksByBank.get(bankId) ?? [];

                items.push(
                    <BankCell key={`bank-${bankId}`}>
                        <BankLabel>Bank {bankId}</BankLabel>
                        {bankChunks.map((chunk, idx) => {
                            // We use Math.max(..., 1) to ensure at least 1% visual height so tiny slivers don't vanish
                            const h = Math.max((1 / MEMORY_LAYOUT.ROWS_PER_BANK) * 100, 1);
                            const top = (chunk.rowId / MEMORY_LAYOUT.ROWS_PER_BANK) * 100;

                            return (
                                <ChunkElement
                                    key={`${bankId}-${idx}`}
                                    bg={chunk.color}
                                    top={top}
                                    h={h}
                                    left={chunk.left}
                                    w={chunk.width}
                                />
                            );
                        })}
                    </BankCell>,
                );
            }
        }

        return items;
    };

    return (
        <ScrollContainer>
            <GridContainer>
                {renderGrid()}
            </GridContainer>
        </ScrollContainer>
    );
});
