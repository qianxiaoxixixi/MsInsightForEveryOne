/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import { useEffect, useState } from 'react';

interface Viewport {
    vw: number;
    vh: number;
}

export function useViewport(): Viewport {
    const [vw, setVw] = useState(window.innerWidth);
    const [vh, setVh] = useState(window.innerHeight);

    useEffect(() => {
        const handleResize = (): void => {
            setVw(window.innerWidth);
            setVh(window.innerHeight);
        };
        window.addEventListener('resize', handleResize);
        return () => window.removeEventListener('resize', handleResize);
    }, []);

    return { vw, vh };
}
