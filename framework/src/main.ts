/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import { createApp } from 'vue';
import { createPinia } from 'pinia';
import App from './App.vue';
import ElementPlus from 'element-plus';
import 'element-plus/dist/index.css';
import 'element-plus/theme-chalk/dark/css-vars.css';
import router from './router';
import '@/i18n';
import '@/assets/main.css';
import directives from './directives';

interface CefQueryType {
    request: string;
    onSuccess: (response: string) => void;
    onFailure: (errorCode: number, errorMessage: string) => void;
}
const app = createApp(App);

app.use(createPinia());
app.use(router);
app.use(ElementPlus, { size: 'small' });

Object.keys(directives).forEach((key) => {
    app.directive(key, directives[key]);
});

app.mount('#app');

// 禁用右键刷新以及F5、Ctrl+R刷新
document.oncontextmenu = (): boolean => false;

document.addEventListener('keydown', (e) => {
    const forbiddenComboKeys = ['f', 'p', 'g', 'j', 'r'];
    const forbiddenSingleKeys = ['F3', 'F5', 'F7'];
    const isCtrlCombo = (e.ctrlKey || e.metaKey) && forbiddenComboKeys.includes(e.key.toLowerCase());

    if (isCtrlCombo || forbiddenSingleKeys.includes(e.key)) {
        e.preventDefault();
    }
});

declare global {
    interface Window {
        cefQuery: (obj: CefQueryType) => void;
    }
}
