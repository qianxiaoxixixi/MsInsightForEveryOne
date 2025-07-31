/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { makeAutoObservable } from 'mobx';
import { RootStore } from './RootStore';
import i18n from 'ascend-i18n';

export type LocaleType = 'zhCN' | 'enUS';

export class SessionStore {
    rootStore: RootStore;
    currentLocale: LocaleType = 'enUS';

    constructor(rootStore: RootStore) {
        this.rootStore = rootStore;
        makeAutoObservable(this);
    }

    setLocale(locale: LocaleType): void {
        this.currentLocale = locale;
        i18n.changeLanguage(locale);
    }
}
