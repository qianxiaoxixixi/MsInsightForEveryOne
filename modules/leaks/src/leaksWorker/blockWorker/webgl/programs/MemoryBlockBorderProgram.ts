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

export class MemoryBlockBorderProgram extends Program {
    protected glInstanceData: Float32Array = new Float32Array();
    protected glInstanceDataSize: number = 0;
    hasBuffer = false;

    bindBuffer(): void {
        const gl = this.gl;
        if (this.instanceBuffer) {
            this.gl.deleteBuffer(this.instanceBuffer);
        }
        this.instanceBuffer = this.createBuffer(4 * this.glInstanceDataSize);
        gl.bindVertexArray(this.vao);
        gl.bindBuffer(gl.ARRAY_BUFFER, this.instanceBuffer);
        const stride = 5 * 4;
        gl.enableVertexAttribArray(0);
        gl.vertexAttribPointer(0, 2, gl.FLOAT, false, stride, 0);
        gl.vertexAttribDivisor(0, 1);
        gl.enableVertexAttribArray(1);
        gl.vertexAttribPointer(1, 2, gl.FLOAT, false, stride, 8);
        gl.vertexAttribDivisor(1, 1);
        gl.enableVertexAttribArray(2);
        gl.vertexAttribPointer(2, 1, gl.FLOAT, false, stride, 16);
        gl.vertexAttribDivisor(2, 1);
        gl.bindVertexArray(null);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
    }

    updateSubBuffer(): void {
        const gl = this.gl;
        gl.bindBuffer(gl.ARRAY_BUFFER, this.instanceBuffer);
        gl.bufferSubData(gl.ARRAY_BUFFER, 0, this.glInstanceData.subarray(0, this.glInstanceDataSize));
    }

    processData(data: RenderData['blocks']): void {
        const totalLength = (data[0] === undefined ? 0 : data[0].path.length + 2) * 5; // +2是为了额外计入左边界和右边界

        const needRealloc = !this.glInstanceData || this.glInstanceData.length < totalLength;

        let instanceData: Float32Array;
        if (needRealloc) {
            // 创建新的buffer
            instanceData = new Float32Array(totalLength);
        } else {
            // 复用已有buffer，但只清空/覆盖前 totalLength 个元素
            instanceData = this.glInstanceData;
        }

        let offset = 0;
        if (data[0] !== undefined) {
            const pathLength = data[0].path.length;
            for (let i = 0; i < pathLength - 1; i++) {
                instanceData[offset++] = data[0].path[i][0];
                instanceData[offset++] = data[0].path[i][1];
                instanceData[offset++] = data[0].path[i + 1][0];
                instanceData[offset++] = data[0].path[i + 1][1];
                instanceData[offset++] = data[0].size;
            }
            // 左边界
            instanceData[offset++] = data[0].path[0][0];
            instanceData[offset++] = data[0].path[0][1];
            instanceData[offset++] = data[0].path[0][0];
            instanceData[offset++] = data[0].path[0][1] + data[0].size;
            instanceData[offset++] = 0;
            // 右边界
            instanceData[offset++] = data[0].path[pathLength - 1][0];
            instanceData[offset++] = data[0].path[pathLength - 1][1];
            instanceData[offset++] = data[0].path[pathLength - 1][0];
            instanceData[offset++] = data[0].path[pathLength - 1][1] + data[0].size;
            instanceData[offset++] = 0;
        }

        this.glInstanceData = instanceData;
        this.glInstanceDataSize = totalLength;
        if (needRealloc) {
            this.bindBuffer();
        } else {
            this.updateSubBuffer();
        }
        this.hasBuffer = true;
    }

    render(options: RenderOptions): void {
        if (!this.hasBuffer || this.instanceBuffer === null) {
            return;
        }
        const gl = this.gl;
        gl.bindBuffer(gl.ARRAY_BUFFER, this.instanceBuffer);
        gl.bufferSubData(gl.ARRAY_BUFFER, 0, this.glInstanceData, 0);
        const instanceCount = this.glInstanceDataSize / 5;
        const uniformData = this.uniformData;
        gl.useProgram(this.program);
        gl.uniform2f(this.uniformLoc.uScale, uniformData[0], uniformData[1]);
        gl.uniform2f(this.uniformLoc.uTranslate, uniformData[2], uniformData[3]);
        gl.uniform2f(this.uniformLoc.uResolution, uniformData[4], uniformData[5]);
        gl.uniform2f(this.uniformLoc.uZoom, uniformData[6], uniformData[7]);
        gl.uniform1f(this.uniformLoc.uOffset, uniformData[8]);
        gl.bindVertexArray(this.vao);
        gl.drawArraysInstanced(gl.LINES, 0, 4, instanceCount);
        gl.bindVertexArray(null);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
    }
}
