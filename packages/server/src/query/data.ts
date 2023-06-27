export type ThreadDetailResponse = {
    emptyFlag: boolean;
    data: {
        title: string;
        selfTime: number;
        duration: number;
        args: string;
    };
};

export type ThreadDetailRequest = {
    pid: string;
    tid: number;
    startTime: number;
    depth: number;
};

export type SliceDao = {
    timestamp: number;
    duration: number;
    name: string;
    args: string;
};

export type MetaData = {
    card: CardMetaData;
    process: ProcessMetaData;
    thread: ThreadMetaData;
};

export type metadataDto = {
    pid: string;
    processName: string;
    tid: number;
    threadName: string;
    maxDepth: number;
};

export type InsightMetaData <T extends keyof MetaData> = {
    type: T;
    metadata: MetaData[T];
    children?: Array<InsightMetaData <keyof MetaData>>;
};

export type ThreadMetaData = {
    cardId?: number;
    processId?: string;
    threadId: number;
    threadName: string;
    maxDepth?: number;
};

export type ProcessMetaData = {
    cardId?: number;
    processId: string;
    processName: string;
};

export type CardMetaData = {
    cardId: number;
    cardName?: string;
};
