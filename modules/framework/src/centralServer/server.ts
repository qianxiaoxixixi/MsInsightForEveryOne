/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import { DataRequest, ModuleName, Host, GLOBAL_HOST, Project } from './websocket/defs';
import { Connection } from './websocket/connection';
import connector from '@/connection';
import { ProjectAction } from '@/utils/enum';
export const CONNECTION_MAP: Map<string, Connection> = new Map();

const getConnectionMapKey = (host: Host): string => {
    return `${host?.remote}:${host?.port}`;
};

export const connectRemote = async function (host: Host): Promise<boolean> {
    const connection = new Connection(host);
    try {
        await connection.connect();
    } catch (e) {
        return false;
    }
    CONNECTION_MAP.set(getConnectionMapKey(host), connection);
    return true;
};

export const isExistedRemote = function(host: Host): boolean {
    return CONNECTION_MAP.has(getConnectionMapKey(host));
};

export const addDataPath = async function(project: Project, action: ProjectAction, isConflict: boolean): Promise<boolean> {
    if (!isExistedRemote(GLOBAL_HOST)) {
        const connected = await connectRemote(GLOBAL_HOST);
        if (!connected) {
            return false;
        }
    }
    const connection = CONNECTION_MAP.get(getConnectionMapKey(GLOBAL_HOST));
    if (connection) {
        connector.send({
            event: 'remote/import',
            body: { dataSource: { ...GLOBAL_HOST, ...project }, isConflict, action },
            target: 'plugin',
        });
    }
    return true;
};

export const request = function (
    moduleName: ModuleName,
    args: DataRequest,
    voidResponse?: boolean,
): Promise<unknown> {
    const connection: Connection | undefined = CONNECTION_MAP.get(getConnectionMapKey(GLOBAL_HOST));
    return new Promise((resolve, reject) => connection?.fetch(moduleName, args, voidResponse)?.then(resolve, reject));
};
