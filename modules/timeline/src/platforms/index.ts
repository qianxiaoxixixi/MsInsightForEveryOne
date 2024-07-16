/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import { Browser } from './Browser';
import type { Platform } from './platform';

export const platform: Platform = new Browser();
