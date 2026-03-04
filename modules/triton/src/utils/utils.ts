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

export const formatTime = (time: number): string => {
    return (time).toFixed(3);
};

// 单位定义（按 1024 进制）
const units = ['B', 'KB', 'MB', 'GB', 'TB'];
const thresholds = [
    1,
    1024,
    1024 * 1024,
    1024 * 1024 * 1024,
    1024 * 1024 * 1024 * 1024,
];

export const formatBytes = (bytes: number): string => {
    const symbol = bytes < 1 ? -1 : 1;
    // 找到合适的单位
    let unitIndex = 0;
    for (let i = thresholds.length - 1; i >= 0; i--) {
        if (bytes * symbol >= thresholds[i]) {
            unitIndex = i;
            break;
        }
    }

    const value = (bytes / thresholds[unitIndex]).toFixed(3);
    return `${value} ${units[unitIndex]}`;
};
