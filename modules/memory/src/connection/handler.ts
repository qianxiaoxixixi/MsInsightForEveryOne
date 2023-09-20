import { store } from '../store';
import { runInAction } from 'mobx';
import { CardInfo } from '../components/ImportSelect';
import { NotificationHandler } from './defs';

const getPropFromData = function<T extends keyof U, U extends Record<string, unknown>>(data: U, key: T): U[T] {
    if (data[key] === undefined) {
        console.warn(`cannot get ${key.toString()} of `, data);
        throw new Error('missed key');
    }
    return data[key];
};

export const importRemoteHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const dataSource = getPropFromData(data, 'dataSource') as DataSource;
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        const result = await window.request(dataSource, { command: 'import/action', params: { path: dataSource.dataPath } });
        runInAction(() => {
            if (!session) {
                return;
            }
            session.token = result.token;
            result.result.forEach((item: CardInfo) => {
                if (!session.memoryRankIds.includes(item.rankId) && (item.hasMemory as boolean)) {
                    session.memoryRankIds.push(item.rankId);
                }
            });
        });
    } catch (err) {
        console.error(err);
    }
};

export const removeRemoteHandler: NotificationHandler = async (data): Promise<void> => {
    try {
        const { sessionStore } = store;
        const session = sessionStore.activeSession;
        runInAction(() => {
            if (!session) {
                return;
            }
            session.memoryRankIds = [];
        });
    } catch (error) {
        console.error(error);
    }
};

export const setTheme: NotificationHandler = (data): void => {
    window.setTheme(Boolean(data.isDark));
};
