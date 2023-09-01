/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

import { Client } from '../types';
import { getLoggerByName } from '../logger/loggger_configure';
import { promises as fsPromises } from 'fs';
import { join } from 'path';

const logger = getLoggerByName('import', 'info');

export type ImportRequest = {
    file: any;
    name: string;
    path: string;
    buffer: any;
};

export type ImportResponse = {
    status: string;
    data: string;
};

async function saveFileFromBytes(fileContent: any, filePath: string): Promise<void> {
    try {
        await fsPromises.writeFile(join(__dirname, filePath), fileContent, { flag: 'w' });
        logger.info('文件下载成功');
    } catch (err) {
        logger.error(err);
    }
}

export const towingImportHandler = async (request: ImportRequest, client: Client): Promise<ImportResponse> => {
    logger.info('接收请求，开始下载文件');
    const { buffer, name } = request;
    let data = Buffer.alloc(0);
    data = Buffer.concat([ data, buffer ], data.length + buffer.length);
    await saveFileFromBytes(data, name);
    const response: ImportResponse = { status: '', data: '' };
    return response;
};
