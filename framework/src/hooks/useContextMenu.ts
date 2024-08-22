/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { onMounted, onUnmounted, type Ref, ref } from 'vue';

interface ContextMenuReturn {
    x: Ref<number>;
    y: Ref<number>;
    visible: Ref<boolean>;
    setVisible: (visible: boolean) => void;
}
export function useContextMenu(container: HTMLElement = document.body, triggerSelector?: string): ContextMenuReturn {
    const x = ref(0);
    const y = ref(0);
    const visible = ref(false);

    const openMenu = (e: MouseEvent): void => {
        e.preventDefault();
        e.stopPropagation();

        if (triggerSelector !== undefined && e.target instanceof Element && !e.target?.matches(triggerSelector)) {
            return;
        }

        visible.value = true;
        x.value = e.clientX;
        y.value = e.clientY;
    };

    const closeMenu = (): void => {
        visible.value = false;
    };

    const setVisible = (_visible: boolean): void => {
        visible.value = _visible;
    };

    onMounted(() => {
        container.addEventListener('contextmenu', openMenu);
        addEventListener('wheel', closeMenu);
        addEventListener('mousedown', closeMenu);
        addEventListener('blur', closeMenu);
    });

    onUnmounted(() => {
        container.removeEventListener('contextmenu', openMenu);
        removeEventListener('wheel', closeMenu);
        removeEventListener('mousedown', closeMenu);
        removeEventListener('blur', closeMenu);
    });

    return { x, y, visible, setVisible };
}
