/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

declare global {
    interface Window {
        setTheme: (isDark: boolean) => void;
        dataSource: DataSource;
    }
}
