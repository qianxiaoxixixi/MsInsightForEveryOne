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
        session.selectedData = undefined;
        session.linkData = undefined;
        session.selectedUnitKeys = [getAutoKey(unit)];
        session.selectedUnits = [unit];
    });
};

export const useSelectUnits = (session: Session): SelectUnit => {
    return (unit: KeyedInsightUnit): void => runInAction(() => {
        if (session.selectedUnits.includes(unit)) {
            return;
        }
        session.selectedUnitKeys.push(getAutoKey(unit));
        session.selectedUnits.push(unit);
    });
};

export const useDeselectUnits = (session: Session): SelectUnit => {
    return (unit: KeyedInsightUnit): void => runInAction(() => {
        const unitKey = getAutoKey(unit);
        const unitIndex = session.selectedUnits.findIndex(selectedUnit => getAutoKey(selectedUnit) === unitKey);

        if (unitIndex !== -1) {
            session.selectedUnits.splice(unitIndex, 1);
            const keyIndex = session.selectedUnitKeys.indexOf(unitKey);
            if (keyIndex !== -1) {
                session.selectedUnitKeys.splice(keyIndex, 1);
            }
        }
    });
};
