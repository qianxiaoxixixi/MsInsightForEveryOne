import { PORT } from './common/common';
import { HANDLER_MAP } from './handlers';
import { InsightServer } from './server/server';

new InsightServer(PORT, HANDLER_MAP).run();
