/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
export enum LocalStorageKeys {
    LANGUAGE = 'language',
    THEME = 'theme',
}

class LocalStorageService {
    getItem(key: LocalStorageKeys): any {
        const item = localStorage.getItem(key);
        return item ? JSON.parse(item) : null;
    }

    setItem(key: LocalStorageKeys, value: any): void {
        localStorage.setItem(key, JSON.stringify(value));
    }

    removeItem(key: LocalStorageKeys): void {
        localStorage.removeItem(key);
    }

    clear(): void {
        localStorage.clear();
    }
}

export const localStorageService = new LocalStorageService();
export default localStorageService;
