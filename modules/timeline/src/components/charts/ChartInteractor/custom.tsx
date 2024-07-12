/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import * as React from 'react';
import type { Theme } from '@emotion/react';
import type { Session } from '../../../entity/session';
import type { Scale } from '../../../entity/chart';

// This is intentionally made a static global variable
const customRenderEffects: CustomRenderEffect[] = [];

export type CustomCrossRenderer = (ctx: CanvasRenderingContext2D | null, session: Session, xScale: Scale, theme: Theme) => void;

export interface CustomRenderEffect {
    action: CustomCrossRenderer;
    triggers: (session: Session) => unknown[];
};

export const registerCrossUnitRenderer = (effect: CustomRenderEffect): void => {
    customRenderEffects.push(effect);
};

export const useCustomRenderers = (session: Session): [ CustomCrossRenderer[], unknown[] ] => {
    const customRenderers = React.useMemo(() => customRenderEffects.map(it => it.action), [session]);
    const customRenderTriggers = customRenderEffects.flatMap(it => it.triggers(session));
    return [customRenderers, customRenderTriggers];
};
