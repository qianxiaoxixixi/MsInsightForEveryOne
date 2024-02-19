/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/

export interface JsonInstructionType {
    Address: string ;
    Source: string ;
    Pipe: string ;
    'Instructions Executed': number[] ;
    Cycles: number[];
};

export interface Iline {
    Line: number;
    Cycle: number;
    'Instruction Executed': number;
    'Address Range': string[][];
};

export interface Ilinetable {
    Line: number;
    Cycles?: number;
    'Instructions Executed'?: number;
    'Address Range'?: string[][];
};

export interface InstrsColumnType {
    Address: string ;
    Source: string ;
    Pipe: string ;
    'Instructions Executed': number;
    Cycles: number;
    maxCycles: number;
};
