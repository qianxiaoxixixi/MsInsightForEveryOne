/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import i18n, { type TFunctionDetailedResult } from 'i18next';
import enUS from './en.json';
import zhCN from './zh.json';

const resources = {
    enUS,
    zhCN,
};
i18n.init({
    resources,
    lng: localStorage.getItem('language') ?? 'enUS',
    interpolation: {
        escapeValue: false, // react already safes from xss
    },
});

type TranslateFunction<T> = (key: string, options?: T) => string | TFunctionDetailedResult<string>;

export const t: TranslateFunction<any> = (key, options) => {
    return i18n.t(key, options);
};

export default i18n;
