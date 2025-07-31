/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

import React from 'react';
import { observer } from 'mobx-react-lite';
import { useTranslation } from 'react-i18next';
import CollapsiblePanel from 'ascend-collapsible-panel';

export const Home: React.FC = observer(() => {
    const { t } = useTranslation('RL');

    return <>
        <CollapsiblePanel title={t('Task Trace Timeline')}>
        </CollapsiblePanel>
    </>;
});
