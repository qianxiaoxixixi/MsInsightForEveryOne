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

export interface MemoryBlock {
    id: string;
    addr: number;
    size: number;
    _startTimestamp: number;
    _endTimestamp: number;
    attr: string;
}

export interface MemoryLifeResponse {
    scope: string;
    status: string;
    errMsg: string;
    blocks: MemoryBlock[];
    maxSize: number;
    maxTimestamp: number;
    mindTimestamp: number;
    total: number;
}

export interface MemorySegmentBlock {
    id: string;
    addr: number;
    offset: number;
    size: number;
    _startTimestamp: number;
    _endTimestamp: number;
    attr: string;
}

export interface MemorySegment {
    address: string;
    totalSize: number;
    callstack: string;
    _startTimestamp: number;
    _endTimestamp: number;
    blocks: MemorySegmentBlock[];
}

export interface MemoryUsageResponse {
    segments: MemorySegment[];
}

export interface BasicInfoResponse {
    kernelName: string;
    scopeTypes: string[];
}
