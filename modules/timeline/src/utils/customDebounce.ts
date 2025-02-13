/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
type Callback<T extends unknown[], K> = (...args: T) => Promise<K>;
export const customDebounce = function<T extends unknown[], K>(callback: Callback<T, K>): Callback<T, K> {
    let waitingTask: ReturnType<Callback<T, K>> | null = null;
    let curTask: Callback<T, K> | null = null;
    const taskQueue: Array<(value: K) => void> = [];
    return (...args) => {
        if (waitingTask === null) {
            waitingTask = callback(...args);
            return waitingTask;
        }
        return new Promise((resolve, reject) => {
            curTask = callback;
            taskQueue.push(resolve);
            waitingTask?.then((res) => {
                if (resolve !== taskQueue[taskQueue.length - 1]) {
                    const index = taskQueue.indexOf(resolve);
                    resolve(res);
                    taskQueue.splice(0, index + 1);
                    return;
                }
                waitingTask = curTask?.(...args) ?? null;
                waitingTask?.then((finalRes) => {
                    const index = taskQueue.indexOf(resolve);
                    resolve(finalRes);
                    taskQueue.splice(0, index + 1);
                });
            })
                .catch(e => reject(e));
        });
    };
};
