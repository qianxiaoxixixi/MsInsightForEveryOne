/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
// Timeline Module
import { light } from '../style/light';
import { dark } from '../style/dark';
import ThemeStore from 'lib/ThemeStore';

export type ThemeItem = 'light' | 'dark';
window.setTheme = (isDark: boolean) => {
    themeInstance.setCurrentTheme(isDark ? 'dark' : 'light');
    document.body.className = isDark ? 'theme_dark' : 'theme_light';
};

export const themeInstance = new ThemeStore({ light, dark });
