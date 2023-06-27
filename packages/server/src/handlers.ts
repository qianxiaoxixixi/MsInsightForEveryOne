import { chartHandler, testHandler } from './handlers.mock';
import { threadTracesHandler } from './trhead.trace.handler';
import { Handler } from './types';
import { importHandler } from './import';
import { unitMetadataHandler } from './query/unitMetadataHandler';

export const HANDLER_MAP: Record<string, Handler> = {
    test: testHandler,
    'unit/chart': chartHandler,
    importCard: importHandler,
    'unit/threadTraces': threadTracesHandler,
    'unit/queryUnitMetadata': unitMetadataHandler,
};
