/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

import {
    AllOperatorsResponse,
    BandwidthDataRequest,
    BandwidthDataResponse,
    DistributionDataRequest,
    DistributionResponse,
    DurationListRequest,
    DurationResponse, Durations,
    IterationsOrRanksResponse,
    OperatorDetailsRequest,
    OperatorNamesRequest,
    OperatorsResponse,
    RanksRequest,
} from './communicationAnalysisData';
import { communicationDbMap } from '../database/tableManager';
import { CommunicationAnalysisDataBase } from '../database/communicationAnalysisDataBase';
import { getLoggerByName } from '../logger/loggger_configure';
const logger = getLoggerByName('communication', 'info');

export const iterationsHandler = async (request: {dbIndex: string}): Promise<IterationsOrRanksResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const response: IterationsOrRanksResponse = { iterationsOrRanks: [] };
    response.iterationsOrRanks = await communicationDB.queryIterationIds() as number[];
    if (response.iterationsOrRanks.length === 0) {
        logger.error('Failed to obtain the number of iteration ids. At least one id must be contained. ' +
            'Check whether communication data files exist in the directory.');
    }
    return response;
};

export const ranksHandler = async (request: RanksRequest): Promise<IterationsOrRanksResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const response: IterationsOrRanksResponse = { iterationsOrRanks: [] };
    response.iterationsOrRanks = await communicationDB.queryRankIds(request.iterationId) as number[];
    return response;
};

export const operatorNamesHandler = async (request: OperatorNamesRequest): Promise<OperatorsResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const response: OperatorsResponse = { operators: [] };
    response.operators = await communicationDB.selectOperators(request.iterationId, request.rankList) as string[];
    return response;
};

export const durationListHandler = async (request: DurationListRequest): Promise<DurationResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const response: DurationResponse = { duration: [] };
    response.duration = await communicationDB.queryDurationList(request.iterationId,
        request.rankList, request.operatorName) as Durations[];
    return response;
};

export const operatorDetailsHandler = async (request: OperatorDetailsRequest): Promise<AllOperatorsResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const operatorNumber = await communicationDB.queryOperatorsCount(request.iterationId, request.rankId);
    if (operatorNumber.length !== 1) {
        logger.error('select operator counts error');
    }
    const response: AllOperatorsResponse = {
        count: operatorNumber[0].nums,
        pageSize: request.pageSize,
        currentPage: request.currentPage,
        allOperators: [],
    };
    response.allOperators = await communicationDB.queryAllOperators(request.iterationId,
        request.rankId, request.pageSize, request.currentPage);
    return response;
};

export const bandwidthHandler = async (request: BandwidthDataRequest): Promise<BandwidthDataResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const response: BandwidthDataResponse = { bandwidthData: [] };
    response.bandwidthData = await communicationDB.queryBandwidthData(request.iterationId, request.rankId,
        request.operatorName);
    if (response.bandwidthData.length !== 4) {
        logger.error('select bandwidth data error. Four types of communication data, ' +
            'such as HCCS, PCIe, SDMA, and RDMA, should be returned.');
    }
    return response;
};

export const distributionHandler = async (request: DistributionDataRequest): Promise<DistributionResponse> => {
    const communicationDB = communicationDbMap.get(request.dbIndex) as CommunicationAnalysisDataBase;
    const response: DistributionResponse = { distributionData: '' };
    response.distributionData = await communicationDB.queryDistributionData(request.iterationId,
        request.rankId, request.operatorName, request.transportType) as string;
    return response;
};
