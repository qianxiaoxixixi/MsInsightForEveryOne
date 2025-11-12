/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { customDebounce } from '../customDebounce';

describe('customDebounce', () => {
    it('should call the callback function immediately if no task is waiting', async () => {
        const mockCallback = jest.fn(() => Promise.resolve('result'));
        const debouncedFunction = customDebounce(mockCallback);

        await debouncedFunction();
        expect(mockCallback).toHaveBeenCalledTimes(1);
    });

    it('should not call the callback function immediately if a task is waiting', async () => {
        const mockCallback = jest.fn(() => Promise.resolve('result'));
        const debouncedFunction = customDebounce(mockCallback);

        debouncedFunction();
        debouncedFunction();
        expect(mockCallback).toHaveBeenCalledTimes(1);
    });

    it('should resolve with the result of the first task if a new task is queued', async () => {
        const mockCallback = jest.fn(() => Promise.resolve('result'));
        const debouncedFunction = customDebounce(mockCallback);

        const result1 = debouncedFunction();
        const result2 = debouncedFunction();

        expect(await result1).toBe('result');
        expect(await result2).toBe('result');
    });

    it('should handle errors from the callback function', async () => {
        const mockCallback = jest.fn(() => Promise.reject(new Error('error')));
        const debouncedFunction = customDebounce(mockCallback);

        try {
            await debouncedFunction();
        } catch (e: any) {
            expect(e).toBeInstanceOf(Error);
            expect(e.message).toBe('error');
        }
    });

    it('should call the callback function again if the previous task is resolved', async () => {
        const mockCallback = jest.fn(() => Promise.resolve('result'));
        const debouncedFunction = customDebounce(mockCallback);

        await debouncedFunction();
        await debouncedFunction();

        expect(mockCallback).toHaveBeenCalledTimes(2);
    });
});
