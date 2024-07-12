/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import { runInAction } from 'mobx';
import type { Session } from '../../../../entity/session';
import { getAutoKey } from '../../../../utils/dataAutoKey';
import type { KeyedInsightUnit } from '../types';

type SelectUnit = (unit: KeyedInsightUnit) => void;
export const useSelectUnit = (session: Session): SelectUnit => {
    return (unit: KeyedInsightUnit): void => runInAction(() => {
        if (session.selectedUnits[0] === unit) { return; }
        session.selectedData = undefined;
        session.linkData = undefined;
        session.selectedUnitKeys = [getAutoKey(unit)];
        session.selectedUnits = [unit];
    });
};
