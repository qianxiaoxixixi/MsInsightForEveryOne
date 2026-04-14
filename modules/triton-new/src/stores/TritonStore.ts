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
import { makeAutoObservable, runInAction } from 'mobx';
import { RootStore } from './RootStore';
import { getMemoryLife, getMemoryUsage, getBasicInfo } from '@/api';
import { MemoryBlock, MemoryLifeResponse, MemoryUsageResponse, BasicInfoResponse } from '@/api/types';

export class TritonStore {
    public rootStore: RootStore;

    public basicInfo: BasicInfoResponse | null = null;
    public selectedMemType: string | null = null;
    public loadingBasicInfo: boolean = false;

    public memoryLifeData: MemoryLifeResponse | null = null;
    public processedBlocks: MemoryBlock[] = [];
    public loadingLife: boolean = false;

    public currentUsageData: MemoryUsageResponse | null = null;
    public loadingUsage: boolean = false;
    public selectedTimestamp: number | null = null;
    public selectedBlock: MemoryBlock | null = null;

    constructor(rootStore: RootStore) {
        this.rootStore = rootStore;
        makeAutoObservable(this);
    }

    public setMemType(type: string): void {
        this.selectedMemType = type;
        this.memoryLifeData = null;
        this.processedBlocks = [];
        this.currentUsageData = null;
        this.selectedTimestamp = null;
        this.selectedBlock = null;
        this.fetchMemoryLife();
    }

    public clearData(): void {
        this.basicInfo = null;
        this.selectedMemType = null;
        this.memoryLifeData = null;
        this.processedBlocks = [];
        this.currentUsageData = null;
        this.selectedTimestamp = null;
        this.selectedBlock = null;
    }

    public async fetchBasicInfo(): Promise<void> {
        this.loadingBasicInfo = true;
        try {
            const data = await getBasicInfo();
            runInAction(() => {
                this.basicInfo = data;
                if (data.scopeTypes && data.scopeTypes.length > 0) {
                    this.selectedMemType = data.scopeTypes[0];
                    this.fetchMemoryLife();
                }
                this.loadingBasicInfo = false;
            });
        } catch (error) {
            runInAction(() => {
                this.loadingBasicInfo = false;
            });
        }
    }

    public setSelectedTimestamp(timestamp: number): void {
        this.selectedTimestamp = Math.floor(timestamp);
    }

    public setSelectedBlock(block: MemoryBlock | null): void {
        this.selectedBlock = block;
    }

    public async fetchMemoryLife(): Promise<void> {
        if (!this.selectedMemType) return;
        this.loadingLife = true;
        try {
            const data = await getMemoryLife(this.selectedMemType);

            runInAction(() => {
                this.memoryLifeData = data;
                this.processedBlocks = data.blocks;
                this.loadingLife = false;
            });
        } catch (error) {
            runInAction(() => {
                this.loadingLife = false;
            });
        }
    }

    public async fetchMemoryUsage(timestamp: number): Promise<void> {
        if (!this.selectedMemType) return;
        this.loadingUsage = true;
        try {
            const data = await getMemoryUsage(timestamp, this.selectedMemType);

            runInAction(() => {
                this.currentUsageData = data;
                this.loadingUsage = false;
            });
        } catch (error) {
            runInAction(() => {
                this.loadingUsage = false;
            });
        }
    }
}
