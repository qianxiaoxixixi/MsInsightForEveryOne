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

export const colors = ['#4e79a7', '#f28e2c', '#e15759', '#76b7b2', '#59a14f', '#edc949', '#af7aa1', '#ff9da7', '#9c755f', '#bab0ab'];
export const highlightColors = ['#8cb3d9', '#f7bc77', '#f08a8c', '#b3dce0', '#9dd68a', '#f5e082', '#e0b3d9', '#ffcdd2', '#d4bfa1', '#e0dbd7'];

const hexToGLColor = (hex: string): [number, number, number, number] => {
    if (!hex) { return [0.5, 0.5, 0.5, 1]; }
    const h = hex.replace('#', '');
    if (h.length !== 6) { return [0.5, 0.5, 0.5, 1]; }
    const r = Number.parseInt(h.slice(0, 2), 16) / 255;
    const g = Number.parseInt(h.slice(2, 4), 16) / 255;
    const b = Number.parseInt(h.slice(4, 6), 16) / 255;
    if (Number.isNaN(r) || Number.isNaN(g) || Number.isNaN(b)) { return [0.5, 0.5, 0.5, 1]; }
    return [r, g, b, 1];
};

export const GL_COLORS: Array<[number, number, number, number]> = colors.map(hexToGLColor);
export const GL_HIGHLIGHT_COLORS: Array<[number, number, number, number]> = highlightColors.map(hexToGLColor);

const hashString = (str: string): number => {
    let hash = 5381;
    for (let i = 0; i < str.length; i++) {
        hash = ((hash << 5) + hash + str.charCodeAt(i)) | 0;
    }
    return hash >>> 0;
};

export const hashHexAddressToIndex = (addr: string): number => {
    const clean = addr.toLowerCase().replace(/^0x/, '');
    return hashString(clean) % GL_COLORS.length;
};

export const getColorStringByAddr = (addr: string, isHightlight: boolean = false): string => {
    const index = hashHexAddressToIndex(addr);
    return isHightlight ? highlightColors[index] : colors[index];
};

export const getColorStringByIndex = (index: number, isHightlight: boolean = false): string => {
    return isHightlight ? highlightColors[index % GL_COLORS.length] : colors[index % GL_COLORS.length];
};
