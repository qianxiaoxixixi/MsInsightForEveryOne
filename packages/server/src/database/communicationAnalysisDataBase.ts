/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

import * as sqlite from 'sqlite3';
import { getLoggerByName } from '../logger/loggger_configure';
const logger = getLoggerByName('communication', 'info');

export class CommunicationAnalysisDataBase {
    private readonly db: sqlite.Database;
    private readonly communicationTimeInfo = 'communication_time_info';
    private readonly communicationBandwidthInfo = 'communication_bandwidth_info';
    private readonly _dbPath: string;

    constructor(dbPath: string) {
        this.db = new sqlite.Database(dbPath, sqlite.OPEN_READWRITE | sqlite.OPEN_CREATE | sqlite.OPEN_SHAREDCACHE, (err) => {
            if (err !== null) {
                logger.error(err.message);
            }
            logger.log('Connect to communication database.');
        });
        this._dbPath = dbPath;
    }

    async executeSql(sql: string, params: any): Promise<any> {
        return new Promise((resolve, reject) => {
            this.db.all(sql, params, function (err, rows) {
                if (err !== undefined && err !== null) {
                    logger.error(err.message);
                    reject(err);
                } else {
                    resolve(rows);
                }
            });
        });
    }

    async queryIterationIds(): Promise<any> {
        const sql: string = `SELECT DISTINCT iteration_id FROM ${this.communicationTimeInfo}`;
        return new Promise((resolve, reject) => {
            this.db.all(sql, function (err, rows) {
                if (err !== undefined && err !== null) {
                    logger.error(err.message);
                    reject(err);
                } else {
                    resolve(rows);
                }
            });
        });
    }

    async queryRankIds(iterationId: number): Promise<any> {
        const sql: string = `SELECT DISTINCT rank_id FROM ${this.communicationTimeInfo}
                                 WHERE iteration_id = ?`;
        return this.executeSql(sql, [iterationId]);
    }

    async selectOperators(iterationId: number, rankIdList: number[]): Promise<any> {
        let sql: string = '';
        if (rankIdList.length === 0) {
            sql = `SELECT DISTINCT op_name FROM (
                   SELECT op_name FROM ${this.communicationTimeInfo}
                                 WHERE iteration_id = ?)`;
        } else {
            sql = `SELECT DISTINCT op_name FROM (
                   SELECT op_name FROM ${this.communicationTimeInfo}
                                 WHERE iteration_id = ?
                                 AND rank_id IN (${rankIdList.join(',')}))`;
        }
        return this.executeSql(sql, [iterationId]);
    }

    async queryDurationList(iterationId: number, rankIdList: number[], operatorName: string): Promise<any> {
        let sql: string = '';
        if (rankIdList.length === 0) {
            sql = `SELECT rank_id, elapse_time, transit_time, synchronization_time, wait_time,
                                  synchronization_time_ratio, wait_time_ratio FROM ${this.communicationTimeInfo}
                                  WHERE iteration_id = ?
                                  AND op_name = ?`;
        } else {
            sql = `SELECT rank_id, elapse_time, transit_time, synchronization_time, wait_time,
                                  synchronization_time_ratio, wait_time_ratio FROM ${this.communicationTimeInfo}
                                  WHERE iteration_id = ?
                                  AND rank_id IN (${rankIdList.join(',')})
                                  AND op_name = ?`;
        }
        return this.executeSql(sql, [ iterationId, operatorName ]);
    };

    async queryAllOperators(iterationId: number, rankId: number, pageSize: number, currentPage: number): Promise<any> {
        const sql: string = `SELECT op_name, elapse_time, transit_time, synchronization_time, wait_time,
                                  synchronization_time_ratio, wait_time_ratio FROM ${this.communicationTimeInfo}
                                  WHERE iteration_id = ?
                                  AND rank_id = ?
                                  LIMIT ?, ?`;
        return this.executeSql(sql, [ iterationId, rankId, (currentPage - 1) * pageSize, pageSize ]);
    }

    async queryOperatorsCount(iterationId: number, rankId: number): Promise<any> {
        const sql: string = `SELECT count(*) AS nums FROM ${this.communicationTimeInfo}
                             WHERE iteration_id = ?
                             AND rank_id = ?`;
        return this.executeSql(sql, [ iterationId, rankId ]);
    }

    async queryBandwidthData(iterationId: number, rankId: number, operatorName: string): Promise<any> {
        const sql: string = `SELECT transport_type, transit_size, transit_time, bandwidth_size,
                             bandwidth_utilization, large_package_ratio FROM ${this.communicationBandwidthInfo}
                             WHERE iteration_id = ?
                             AND rank_id = ?
                             AND op_name = ?`;
        return this.executeSql(sql, [ iterationId, rankId, operatorName ]);
    }

    async queryDistributionData(iterationId: number, rankId: number, operatorName: string, transportType: string): Promise<any> {
        const sql: string = `SELECT size_distribution FROM ${this.communicationBandwidthInfo}
                             WHERE iteration_id = ?
                             AND rank_id = ?
                             AND op_name = ?
                             AND transport_type = ?`;
        return this.executeSql(sql, [ iterationId, rankId, operatorName, transportType ]);
    }
}
