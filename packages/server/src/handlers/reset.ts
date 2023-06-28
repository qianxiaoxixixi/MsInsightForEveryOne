import { importedRankIdSet } from './import';

export const resetHandler = async (req: any): Promise<Record<string, unknown>> => {
    importedRankIdSet.clear();
    return { };
};
