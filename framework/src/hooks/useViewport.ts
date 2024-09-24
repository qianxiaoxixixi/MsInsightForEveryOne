/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { type Ref, ref } from 'vue';

const vw = ref(document.documentElement.clientWidth);
const vh = ref(document.documentElement.clientHeight);

window.addEventListener('resize', () => {
    vw.value = document.documentElement.clientWidth;
    vh.value = document.documentElement.clientHeight;
});

export function useViewport(): { vw: Ref<number>; vh: Ref<number> } {
    return {
        vw,
        vh,
    };
}
