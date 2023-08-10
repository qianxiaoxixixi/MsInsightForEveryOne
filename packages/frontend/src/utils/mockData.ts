/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
export const computationCommunicationData: any = {
    top: [
        {
            rankId: '0',
            totalComputeTime: 1,
            totalCommunicationNotOverLapTime: 2,
            totalCommunicationTime: 3,
            totalFreeTime: 4,
            totalPureCommunicationTime: 5,
            ComputeTimeRatio: 20,
            CommunicationTimeRatio: 80,
        },
        {
            rankId: '1',
            totalComputeTime: 1,
            totalCommunicationNotOverLapTime: 2,
            totalCommunicationTime: 3,
            totalFreeTime: 4,
            totalPureCommunicationTime: 5,
            ComputeTimeRatio: 50,
            CommunicationTimeRatio: 50,
        },
        {
            rankId: '2',
            totalComputeTime: 1,
            totalCommunicationNotOverLapTime: 2,
            totalCommunicationTime: 3,
            totalFreeTime: 4,
            totalPureCommunicationTime: 5,
            ComputeTimeRatio: 80,
            CommunicationTimeRatio: 20,
        },
        {
            rankId: '3',
            totalComputeTime: 4,
            totalCommunicationNotOverLapTime: 2,
            totalCommunicationTime: 1,
            totalFreeTime: 9,
            totalPureCommunicationTime: 3,
            ComputeTimeRatio: 80,
            CommunicationTimeRatio: 20,
        },
    ],
    ComputingDetail: [
        {
            Name: 'AsStrided',
            Type: 'AsStrided',
            'Start Time': '2022-07-21 17:30:22',
            'Duration(us)': 7.06,
            'Wait Time(us)': 0,
            'Block Dim': 3,
            'Input Shapes': '8196;2;2;',
            'Input Data Types': 'INT64;INT64;INT64;INT64',
            'Input Formats': 'FORMAT_ND;FORMAT_ND;FORMAT_ND;FORMAT_ND',
            'Output Shapes': '4,2048',
            'Output Data Types': 'INT64',
            'Output Formats': 'FORMAT_ND',
        },
        {
            Name: 'OnesLike',
            Type: 'OnesLike',
            'Start Time': '2022-07-21 17:31:22',
            'Duration(us)': 11.86,
            'Wait Time(us)': 637.5599,
            'Block Dim': 48,
            'Input Shapes': '1,2048,2048',
            'Input Data Types': 'FLOAT',
            'Input Formats': 'FORMAT_ND',
            'Output Shapes': '1,2048,2048',
            'Output Data Types': 'FLOAT',
            'Output Formats': 'FORMAT_ND',
        },
    ],
    'Communication(OverLapped)Detail': [
        {
            'Communication Kernel': 'all_reduce1',
            'Start Time': '2022-07-21 17:30:22',
            'Total Durations(us)': 344.1,
            'Overlapped Durations(us)': 333.2,
            'Not Overlapped Durations(us)': 21.4,
        },
        {
            'Communication Kernel': 'all_reduce2',
            'Start Time': '2022-07-21 17:33:22',
            'Total Durations(us)': 211.1,
            'Overlapped Durations(us)': 100,
            'Not Overlapped Durations(us)': 30,
        },
    ],
    'Communication(Not OverLapped)Detail': [
        {
            'Communication Kernel': 'all_reduce1',
            'Start Time': '2022-07-21 17:30:22',
            'Total Durations(us)': 344.1,
            'Overlapped Durations(us)': 333.2,
            'Not Overlapped Durations(us)': 21.4,
        },
        {
            'Communication Kernel': 'all_reduce2',
            'Start Time': '2022-07-21 17:33:22',
            'Total Durations(us)': 211.1,
            'Overlapped Durations(us)': 100,
            'Not Overlapped Durations(us)': 30,
        },
    ],
    Computing: [
        {
            'Accelerator Core': 'AI CORE',
            'Accelerator Core Durations(us)': 500000,
            'Accelerator Core Utilization': '80%',
        },
        {
            'Accelerator Core': 'AI CPU',
            'Accelerator Core Durations(us)': 3222,
            'Accelerator Core Utilization': '20%',
        },
    ],
    'Communication(Not OverLapped)': [
        {
            'Transport Type': 'SDMA',
            'Total Durations(us)': 50000,
        },
        {
            'Transport Type': 'RDMA',
            'Total Durations(us)': 320,
        },
        {
            'Transport Type': 'LOCAL',
            'Total Durations(us)': 111,
        },
    ],
    'Communication(OverLapped)': [
        {
            'Transport Type': 'SDMA',
            'Total Durations(us)': 50000,
        },
        {
            'Transport Type': 'RDMA',
            'Total Durations(us)': 320,
        },
        {
            'Transport Type': 'LOCAL',
            'Total Durations(us)': 111,
        },
    ],
};
