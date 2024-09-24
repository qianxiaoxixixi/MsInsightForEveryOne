/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import type { DirectiveBinding } from 'vue';

const map = new WeakMap();

const ob = new ResizeObserver((entries) => {
    for (const entry of entries) {
        const handler = map.get(entry.target);
        if (handler) {
            handler({
                width: entry.borderBoxSize[0].inlineSize,
                height: entry.borderBoxSize[0].blockSize,
            });
        }
    }
});

export default {
    mounted(el: HTMLElement, binding: DirectiveBinding): void {
        map.set(el, binding.value);
        ob.observe(el);
    },

    unmounted(el: HTMLElement): void {
        ob.unobserve(el);
    },
};
