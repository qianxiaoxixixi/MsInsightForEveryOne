import { notificationTestHandler } from './connection/handler.mock';
import { NotificationHandler } from './connection/defs';

type InsightInterface<Request extends Record<string, unknown>, Response extends Record<string, unknown>> = {
    request: Request;
    response: Response;
};

export type InterfaceDefs = {
    'chart/cpu': InsightInterface<{ chartId: number }, { data: Array<{ ts: number; value: number }>}>;
};

export const NOTIFICATION_HANDLERS: Record<string, NotificationHandler> = {
    'test/notify': notificationTestHandler,
};
