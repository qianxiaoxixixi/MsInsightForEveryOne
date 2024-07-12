/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
*/

import React, { useMemo, useState } from 'react';
import { useTranslation } from 'react-i18next';
import { observer } from 'mobx-react';
import type { Session } from '../../entity/session';
import { useEventBus } from '../../utils/eventBus';
import { ViewList } from './index';

export function getDetailViewItem(session: Session, bottomHeight: number): any {
    if (!session.isSimulation) {
        return {
            label: <ViewSelect/>,
            key: 'SystemView',
            children: <ViewContainer session={session} bottomHeight={bottomHeight} />,
        };
    } else {
        return {};
    }
}

const ViewSelect = observer((props: any) => {
    const { t } = useTranslation('timeline');
    return (<div className={'title'}>{<span>{t('System View')}</span>}</div>);
});

const ViewContainer = observer((props: any) => {
    const [view, setView] = useState(0);
    useEventBus('setView', (data) => setView(data as number));
    const View = useMemo(() => ViewList[view], [view]);
    return (<View session={props.session} bottomHeight={props.bottomHeight} />);
});
