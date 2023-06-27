import { Client } from './types';
import { InsightError } from './utils/error';

export const testHandler = async (reqData: Record<string, unknown>, client: Client): Promise<Record<string, unknown>> => {
    if (reqData.innerData !== undefined) {
        const delay = 3000;

        // simulates some time-consuming work that can not be returned immediately, use notifications with promise+then
        new Promise<void>(resolve => {
            setTimeout(() => {
                resolve();
            }, delay);
        }).then(() => {
            // client will receive this notification message
            client.notify('test/notify', { info: `server notification after ${delay} ms` });
        });

        // the immediate result that can be used to fulfill a client request
        return {
            state: 'ok',
        };
    }
    throw new InsightError(123, 'error message 123');
};

export const chartHandler = async (req: { pid: number; chartId: number }, client: Client): Promise<Record<string, unknown>> => {
    return {
        data: [{ ts: 0, value: 1 }],
    };
};
