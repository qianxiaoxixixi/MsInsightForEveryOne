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

import { Program } from './Program';

export enum RenderType {
    border = 'border',
    highlightBorder = 'highlightBorder',
};

export class MemoryStateBorderProgram extends Program {
    readonly renderType: RenderType;
    protected glInstanceData: Float32Array = new Float32Array();
    protected glInstanceDataSize: number = 0;
    hasBuffer = false;
    protected highlightAlpha: number = 0;
    protected stride = 3;

    constructor(gl: WebGL2RenderingContext, uniformData: Float32Array, shader: Shader, renderType: RenderType = RenderType.border) {
        super(gl, uniformData, shader);
        this.renderType = renderType;
    }

    bindBuffer(): void {
        const gl = this.gl;
        if (this.instanceBuffer) {
            gl.deleteBuffer(this.instanceBuffer);
        }
        this.instanceBuffer = this.createBuffer(4 * this.glInstanceDataSize);
        gl.bindVertexArray(this.vao);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.instanceBuffer);
        const strideBytes = this.stride * 4;
        gl.enableVertexAttribArray(0);
        gl.vertexAttribPointer(0, 1, gl.FLOAT, false, strideBytes, 0);
        gl.vertexAttribDivisor(0, 1);
        gl.enableVertexAttribArray(1);
        gl.vertexAttribPointer(1, 1, gl.FLOAT, false, strideBytes, 4);
        gl.vertexAttribDivisor(1, 1);
        gl.enableVertexAttribArray(2);
        gl.vertexAttribPointer(2, 1, gl.FLOAT, false, strideBytes, 8);
        gl.vertexAttribDivisor(2, 1);
        this.cleanupGL();
    }

    processData(data: Segment[]): void {
        if (this.renderType !== RenderType.border) {
            return;
        }
        const totalLength = data.length * this.stride;
        const needRealloc = !this.glInstanceData || this.glInstanceData.length < totalLength;
        const instanceData = needRealloc ? new Float32Array(totalLength) : this.glInstanceData;

        let offset = 0;
        for (let i = 0; i < data.length; i++) {
            const segment = data[i];
            instanceData[offset++] = segment.offsetX;
            instanceData[offset++] = segment.offsetY;
            instanceData[offset++] = segment.size;
        }

        this.glInstanceData = instanceData;
        this.glInstanceDataSize = totalLength;
        if (needRealloc) {
            this.bindBuffer();
        } else {
            this.updateSubBuffer(instanceData, totalLength);
        }
        this.hasBuffer = true;
    }

    processHighlightData(highlightData: StateDataHoverResult | null): void {
        if (this.renderType !== RenderType.highlightBorder) {
            return;
        }
        if (this.glInstanceDataSize < this.stride) {
            this.glInstanceData = new Float32Array(this.stride);
            this.glInstanceDataSize = this.stride;
            this.bindBuffer();
        }

        const instanceData = this.glInstanceData;
        if (highlightData?.type === 'segment') {
            const segment = highlightData.data;
            instanceData[0] = segment.offsetX;
            instanceData[1] = segment.offsetY;
            instanceData[2] = segment.size;
            this.highlightAlpha = 1;
        } else if (highlightData?.type === 'block') {
            const segment = highlightData.data;
            const block = segment.blocks[0];
            instanceData[0] = segment.offsetX + block.offset;
            instanceData[1] = segment.offsetY;
            instanceData[2] = block.size;
            this.highlightAlpha = 1;
        } else {
            instanceData[0] = 0;
            instanceData[1] = 0;
            instanceData[2] = 0;
            this.highlightAlpha = 0;
        }
        this.hasBuffer = true;
    }

    render(options: RenderOptions): void {
        if (!this.hasBuffer || this.instanceBuffer === null) {
            return;
        }
        const gl = this.gl;
        this.updateSubBuffer(this.glInstanceData, this.glInstanceDataSize);
        gl.useProgram(this.program);
        this.setBaseUniforms();
        const borderColor = this.renderType === RenderType.highlightBorder
            ? [1, 0, 0, this.highlightAlpha]
            : [0, 0, 0, 1];
        gl.uniform4f(this.uniformLoc.uBorderColor, borderColor[0], borderColor[1], borderColor[2], borderColor[3]);
        gl.bindVertexArray(this.vao);
        gl.drawArraysInstanced(gl.LINES, 0, 8, this.glInstanceDataSize / this.stride);
        this.cleanupGL();
    }
}
