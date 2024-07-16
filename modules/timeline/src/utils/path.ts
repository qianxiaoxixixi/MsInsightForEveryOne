/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
export function fileNameOfPath(path: string): string {
    return path.substring(1 + path.lastIndexOf('/'));
}
